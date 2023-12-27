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
#include "chrohime/api/lifetime/chrohime_views_delegate.h"
#include "mojo/core/embedder/embedder.h"
#include "ui/accessibility/platform/ax_platform_for_test.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/compositor/test/test_context_factories.h"
#include "ui/display/screen.h"
#include "ui/gfx/font_util.h"
#include "ui/gl/gl_utils.h"
#include "ui/gl/init/gl_factory.h"

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
#include "ui/aura/env.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "ui/wm/core/wm_state.h"
#endif

#if BUILDFLAG(IS_OZONE)
#include "ui/ozone/public/ozone_platform.h"
#endif

#if BUILDFLAG(IS_LINUX)
#include "ui/linux/linux_ui.h"
#include "ui/linux/linux_ui_factory.h"
#endif

#if BUILDFLAG(IS_WIN)
#include "base/win/win_util.h"
#include "ui/base/win/scoped_ole_initializer.h"
#endif

namespace hime {

struct LifetimeImpl {
  base::AtExitManager at_exit;
#if BUILDFLAG(IS_WIN)
  ui::ScopedOleInitializer ole_initializer;
#endif
  ui::AXPlatformForTest ax_platform;
  std::unique_ptr<base::test::ScopedDisableRunLoopTimeout> disable_timeout;
  std::unique_ptr<base::test::TaskEnvironment> task_environment;
  std::unique_ptr<ui::TestContextFactories> context_factories;
#if BUILDFLAG(IS_MAC)
  display::ScopedNativeScreen desktop_screen_;
#endif
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  std::unique_ptr<aura::Env> aura_env;
  std::unique_ptr<wm::WMState> wm_state;
  std::unique_ptr<display::Screen> desktop_screen;
#endif
  std::unique_ptr<base::RunLoop> run_loop;
};

int Lifetime::RunMain() {
  impl_->run_loop->Run();
  OnPostMainMessageLoopRun();
  return 0;
}

void Lifetime::OnPreMainMessageLoopRun(base::RepeatingClosure quit_closure) {
  quit_closure_ = std::move(quit_closure);

  views_delegate_ = std::make_unique<ChrohimeViewsDelegate>();
#if BUILDFLAG(IS_MAC)
  views_delegate_->set_context_factory(
      impl_->context_factories->GetContextFactory());
#endif
}

void Lifetime::OnPostMainMessageLoopRun() {
  views_delegate_.reset();
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

#if BUILDFLAG(IS_MAC)
  OnPreBrowserMain();
#endif

  // Viz depends on the task environment to correctly tear down.
  impl_->task_environment.reset(new base::test::TaskEnvironment(
      base::test::TaskEnvironment::MainThreadType::UI));

#if BUILDFLAG(IS_OZONE)
  ui::OzonePlatform::InitParams params;
  params.single_process = true;
  ui::OzonePlatform::InitializeForUI(params);
  ui::OzonePlatform::GetInstance()->PostCreateMainMessageLoop(
      base::BindOnce([] { LOG(FATAL) << "Failed to shutdown."; }),
      base::SingleThreadTaskRunner::GetCurrentDefault());
  ui::OzonePlatform::InitializeForGPU(params);
#endif

  gl::init::InitializeGLOneOff(/*gpu_preference=*/gl::GpuPreference::kDefault);

  // The ContextFactory must exist before any Compositors are created.
  impl_->context_factories.reset(new ui::TestContextFactories(
      /*under_test*/false, /*output_to_window=*/true));

  base::i18n::InitializeICU();

  base::FilePath resources_pak_path;
  base::PathService::Get(base::DIR_ASSETS, &resources_pak_path);
  ui::ResourceBundle::InitSharedInstanceWithPakPath(
      resources_pak_path.AppendASCII("chrohime.pak"));

#if BUILDFLAG(IS_WIN)
  base::win::EnableHighDPISupport();
#elif BUILDFLAG(IS_LINUX)
  ui::LinuxUi::SetInstance(ui::GetDefaultLinuxUi());
#endif

  gfx::InitializeFonts();

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  impl_->aura_env = aura::Env::CreateInstance();
  impl_->aura_env->set_context_factory(
      impl_->context_factories->GetContextFactory());
  impl_->wm_state.reset(new wm::WMState);
  impl_->desktop_screen = views::CreateDesktopScreen();
#endif

  impl_->disable_timeout.reset(new base::test::ScopedDisableRunLoopTimeout);
  impl_->run_loop.reset(
      new base::RunLoop(base::RunLoop::Type::kNestableTasksAllowed));

  OnPreMainMessageLoopRun(impl_->run_loop->QuitClosure());

#if !BUILDFLAG(IS_MAC)
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce([](base::WeakPtr<Lifetime> self) {
        if (self)
          self->on_ready.Emit();
      }, GetWeakPtr()));
#endif
}

void Lifetime::Destroy() {
  delete impl_.get();
}

}  // namespace hime
