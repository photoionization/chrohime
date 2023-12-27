// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime.h"

#include <iostream>

#include "base/check.h"
#include "base/command_line.h"
#include "base/run_loop.h"
#include "chrohime/api/lifetime/chrohime_views_delegate.h"
#include "ui/base/ime/init/input_method_initializer.h"
#include "ui/display/screen.h"

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "ui/wm/core/wm_state.h"
#endif

#if BUILDFLAG(IS_LINUX)
#include "ui/linux/linux_ui.h"
#include "ui/linux/linux_ui_factory.h"
#endif

namespace hime {

namespace {

thread_local Lifetime* g_lifetime = nullptr;

}  // namespace

// static
Lifetime* Lifetime::GetCurrent() {
  return g_lifetime;
}

#if BUILDFLAG(IS_WIN)
Lifetime::Lifetime() {
  bool success = base::CommandLine::Init(0, nullptr);
#else
Lifetime::Lifetime(int argc, const char** argv) {
  bool success = base::CommandLine::Init(argc, argv);
#endif
  CHECK(success) << "Failed to initialize CommandLine, has Lifetime been "
                    "created twice, or is the code running in Chromium?";
  g_lifetime = this;

#if BUILDFLAG(IS_WIN)
  Initialize();
#else
  Initialize(argc, argv);
#endif
}

Lifetime::~Lifetime() {
  g_lifetime = nullptr;
  Destroy();
}

void Lifetime::Quit() {
  quit_closure_.Run();
}

void Lifetime::OnPreMainMessageLoopRun() {
  run_loop_.reset(
      new base::RunLoop(base::RunLoop::Type::kNestableTasksAllowed));
  quit_closure_ = run_loop_->QuitClosure();

#if BUILDFLAG(IS_LINUX)
  ui::LinuxUi::SetInstance(ui::GetDefaultLinuxUi());
#endif
  ui::InitializeInputMethodForTesting();

#if BUILDFLAG(IS_MAC)
  desktop_screen_ = std::make_unique<display::ScopedNativeScreen>();
#endif
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  wm_state_ = std::make_unique<wm::WMState>();
  screen_ = views::CreateDesktopScreen();
#endif
  views_delegate_ = std::make_unique<ChrohimeViewsDelegate>();

  impl_->OnPreMainMessageLoopRun();
}

void Lifetime::OnWillRunMainMessageLoop(
    std::unique_ptr<base::RunLoop>& run_loop) {
  run_loop = std::move(run_loop_);
}

void Lifetime::OnPostMainMessageLoopRun() {
  impl_->OnPostMainMessageLoopRun();

  views_delegate_.reset();
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  screen_.reset();
#endif
#if BUILDFLAG(IS_LINUX)
  ui::LinuxUi::SetInstance(nullptr);
#endif
}

}  // namespace hime
