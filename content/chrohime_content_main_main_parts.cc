// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/content/chrohime_content_main_main_parts.h"

#include "base/run_loop.h"
#include "chrohime/content/chrohime_content_client.h"
#include "content/public/common/result_codes.h"
#include "content/shell/browser/shell_browser_context.h"
#include "ui/base/ime/init/input_method_initializer.h"
#include "ui/views/test/desktop_test_views_delegate.h"

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
#include "ui/display/screen.h"
#include "ui/views/widget/desktop_aura/desktop_screen.h"
#include "ui/wm/core/wm_state.h"
#endif

#if BUILDFLAG(IS_LINUX)
#include "ui/linux/linux_ui.h"
#include "ui/linux/linux_ui_factory.h"
#endif

#if BUILDFLAG(IS_MAC)
#include "content/public/browser/context_factory.h"
#endif

namespace hime {

ChrohimeContentClientMainParts::ChrohimeContentClientMainParts(
    ChrohimeContentClient* content_client) : content_client_(content_client) {}

ChrohimeContentClientMainParts::~ChrohimeContentClientMainParts() = default;

void ChrohimeContentClientMainParts::ToolkitInitialized() {
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  wm_state_ = std::make_unique<wm::WMState>();
#endif
#if BUILDFLAG(IS_LINUX)
  ui::LinuxUi::SetInstance(ui::GetDefaultLinuxUi());
#endif
}

int ChrohimeContentClientMainParts::PreMainMessageLoopRun() {
  ui::InitializeInputMethodForTesting();
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  screen_ = views::CreateDesktopScreen();
#endif
  run_loop_ = std::make_unique<base::RunLoop>();
  views_delegate_ = std::make_unique<views::DesktopTestViewsDelegate>();
#if BUILDFLAG(IS_MAC)
  views_delegate_->set_context_factory(content::GetContextFactory());
#endif
  browser_context_ = std::make_unique<content::ShellBrowserContext>(false);
  content_client_->OnPreMainMessageLoopRun(browser_context_.get());
  return content::RESULT_CODE_NORMAL_EXIT;
}

void ChrohimeContentClientMainParts::WillRunMainMessageLoop(
    std::unique_ptr<base::RunLoop>& run_loop) {
  run_loop = std::move(run_loop_);
}

void ChrohimeContentClientMainParts::PostMainMessageLoopRun() {
  browser_context_.reset();
  views_delegate_.reset();
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  screen_.reset();
#endif
#if BUILDFLAG(IS_LINUX)
  ui::LinuxUi::SetInstance(nullptr);
#endif
}

}  // namespace hime
