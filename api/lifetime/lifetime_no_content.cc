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
#include "ui/gfx/font_util.h"
#include "ui/gl/gl_utils.h"
#include "ui/gl/init/gl_factory.h"

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
#include "ui/aura/env.h"
#endif

#if BUILDFLAG(IS_OZONE)
#include "ui/ozone/public/ozone_platform.h"
#endif

#if BUILDFLAG(IS_WIN)
#include "base/win/win_util.h"
#include "ui/base/win/scoped_ole_initializer.h"
#endif

namespace hime {

struct LifetimeImpl : public ContentLifetimeDelegate {
  base::AtExitManager at_exit;
#if BUILDFLAG(IS_WIN)
  ui::ScopedOleInitializer ole_initializer;
#endif
  ui::AXPlatformForTest ax_platform;
  std::unique_ptr<base::test::TaskEnvironment> task_environment;
  std::unique_ptr<base::test::ScopedDisableRunLoopTimeout> disable_timeout;
  std::unique_ptr<ui::TestContextFactories> context_factories;
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  std::unique_ptr<aura::Env> aura_env;
#endif
};

int Lifetime::RunMain() {
  run_loop_->Run();
  OnPostMainMessageLoopRun();
  return 0;
}

#if BUILDFLAG(IS_WIN)
void Lifetime::Initialize() {
#else
void Lifetime::Initialize(int argc, const char** argv) {
#endif
  LifetimeImpl* impl = new LifetimeImpl;
  impl_ = impl;

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
  impl->task_environment.reset(new base::test::TaskEnvironment(
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
  impl->context_factories.reset(new ui::TestContextFactories(
      /*under_test*/false, /*output_to_window=*/true));

  base::i18n::InitializeICU();

  base::FilePath resources_pak_path;
  base::PathService::Get(base::DIR_ASSETS, &resources_pak_path);
  ui::ResourceBundle::InitSharedInstanceWithPakPath(
      resources_pak_path.AppendASCII("chrohime.pak"));

#if BUILDFLAG(IS_WIN)
  base::win::EnableHighDPISupport();
#endif

  gfx::InitializeFonts();

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  impl->aura_env = aura::Env::CreateInstance();
  impl->aura_env->set_context_factory(
      impl->context_factories->GetContextFactory());
#endif

  impl->disable_timeout.reset(new base::test::ScopedDisableRunLoopTimeout);

  OnPreMainMessageLoopRun();
#if BUILDFLAG(IS_MAC)
  views_delegate_->set_context_factory(
      impl->context_factories->GetContextFactory());
#endif

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
