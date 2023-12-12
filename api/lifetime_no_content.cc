// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime.h"

#include "base/at_exit.h"
#include "base/i18n/icu_util.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "base/test/task_environment.h"
#include "base/test/test_timeouts.h"
#include "mojo/core/embedder/embedder.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/compositor/test/test_context_factories.h"
#include "ui/gfx/font_util.h"
#include "ui/gl/gl_utils.h"
#include "ui/gl/init/gl_factory.h"
#include "ui/views/test/desktop_test_views_delegate.h"

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
#include "ui/display/screen.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#endif

#if BUILDFLAG(IS_OZONE)
#include "ui/ozone/public/ozone_platform.h"
#endif

#if BUILDFLAG(IS_WIN)
#include "ui/base/win/scoped_ole_initializer.h"
#endif

#if defined(USE_AURA)
#include "ui/aura/env.h"
#include "ui/wm/core/wm_state.h"
#endif

namespace hime {

struct LifetimeImpl {
  base::AtExitManager at_exit;
#if BUILDFLAG(IS_WIN)
  ui::ScopedOleInitializer ole_initializer;
#endif
  views::DesktopTestViewsDelegate views_delegate;
  std::unique_ptr<base::test::ScopedDisableRunLoopTimeout> disable_timeout;
  std::unique_ptr<base::test::TaskEnvironment> task_environment;
  std::unique_ptr<ui::TestContextFactories> context_factories;
#if defined(USE_AURA)
  std::unique_ptr<aura::Env> aura_env;
  std::unique_ptr<wm::WMState> wm_state;
#endif
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  std::unique_ptr<display::Screen> desktop_screen;
#endif
  std::unique_ptr<base::RunLoop> run_loop;
};

int Lifetime::RunMain() {
  impl_->run_loop->Run();
  return 0;
}

#if BUILDFLAG(IS_WIN)
void Lifetime::Initialize() {
#else
void Lifetime::Initialize(int argc, const char** argv) {
#endif
  impl_ = new LifetimeImpl;

  // The use of base::test::TaskEnvironment in the following function relies on
  // the timeout values from TestTimeouts.
  TestTimeouts::Initialize();

  // Disabling Direct Composition works around the limitation that
  // InProcessContextFactory doesn't work with Direct Composition, causing the
  // window to not render. See http://crbug.com/936249.
  gl::SetGlWorkarounds(gl::GlWorkarounds{.disable_direct_composition = true});

  mojo::core::Init();

#if BUILDFLAG(IS_OZONE)
  ui::OzonePlatform::InitParams params;
  params.single_process = true;
  ui::OzonePlatform::InitializeForGPU(params);
#endif

  gl::init::InitializeGLOneOff(/*gpu_preference=*/gl::GpuPreference::kDefault);

  // Viz depends on the task environment to correctly tear down.
  impl_->task_environment.reset(new base::test::TaskEnvironment(
      base::test::TaskEnvironment::MainThreadType::UI));

  // The ContextFactory must exist before any Compositors are created.
  impl_->context_factories.reset(new ui::TestContextFactories(
      /*under_test*/false, /*output_to_window=*/true));

  base::i18n::InitializeICU();

  ui::RegisterPathProvider();

  base::FilePath ui_test_pak_path;
  CHECK(base::PathService::Get(ui::UI_TEST_PAK, &ui_test_pak_path));
  ui::ResourceBundle::InitSharedInstanceWithPakPath(ui_test_pak_path);

  base::FilePath resources_pak_path;
  CHECK(base::PathService::Get(base::DIR_ASSETS, &resources_pak_path));
  ui::ResourceBundle::GetSharedInstance().AddDataPackFromPath(
      resources_pak_path.AppendASCII("chrohime_resources.pak"),
      ui::k100Percent);

  gfx::InitializeFonts();

#if defined(USE_AURA)
  impl_->aura_env = aura::Env::CreateInstance();
  impl_->aura_env->set_context_factory(
      impl_->context_factories->GetContextFactory());
  impl_->wm_state.reset(new wm::WMState);
#endif
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  impl_->desktop_screen = views::CreateDesktopScreen();
#endif

  impl_->disable_timeout.reset(new base::test::ScopedDisableRunLoopTimeout);
  impl_->run_loop.reset(
      new base::RunLoop(base::RunLoop::Type::kNestableTasksAllowed));

  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce([](base::WeakPtr<Lifetime> self) {
        if (self)
          self->on_ready.Emit();
      }, GetWeakPtr()));
}

void Lifetime::Destroy() {
  delete impl_.get();
}

}  // namespace hime
