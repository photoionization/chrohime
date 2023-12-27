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
  std::unique_ptr<base::RunLoop> run_loop_;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_MAIN_PARTS_H_
