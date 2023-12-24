// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_MAIN_PARTS_H_
#define CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_MAIN_PARTS_H_

#include "chrohime/chrohime_export.h"
#include "content/public/browser/browser_main_parts.h"
#include "ui/display/screen.h"
#include "ui/views/buildflags.h"

namespace base {
class RunLoop;
}

namespace content {
class ShellBrowserContext;
}

namespace views {
class TestViewsDelegate;
}

namespace wm {
class WMState;
}

namespace hime {

class ContentLifetimeDelegate;

class CHROHIME_EXPORT ContentLifetimeDelegateMainParts
    : public content::BrowserMainParts {
 public:
  explicit ContentLifetimeDelegateMainParts(
      ContentLifetimeDelegate* delegate);
  ~ContentLifetimeDelegateMainParts() override;

  // content::BrowserMainParts:
  void ToolkitInitialized() override;
  int PreMainMessageLoopRun() override;
  void WillRunMainMessageLoop(
      std::unique_ptr<base::RunLoop>& run_loop) override;
  void PostMainMessageLoopRun() override;

 private:
  raw_ptr<ContentLifetimeDelegate> delegate_;
#if BUILDFLAG(IS_MAC)
  display::ScopedNativeScreen desktop_screen_;
#endif
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  std::unique_ptr<wm::WMState> wm_state_;
  std::unique_ptr<display::Screen> screen_;
#endif
  std::unique_ptr<views::TestViewsDelegate> views_delegate_;
  std::unique_ptr<base::RunLoop> run_loop_;
  // The browser_context is a member of main_parts because the content_shell_lib
  // dependency is a static_library and so we can't use ShellBrowserContext in
  // lifetime_with_content directly.
  // TODO(zcbenz): Get rid of ShellBrowserContext and create browser_context in
  // lifetime_with_content instead.
  std::unique_ptr<content::ShellBrowserContext> browser_context_;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_MAIN_PARTS_H_
