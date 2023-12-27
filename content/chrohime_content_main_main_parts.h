// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_MAIN_PARTS_H_
#define CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_MAIN_PARTS_H_

#include "chrohime/chrohime_export.h"
#include "content/public/browser/browser_main_parts.h"

namespace hime {

class ContentLifetimeDelegate;

class CHROHIME_EXPORT ContentLifetimeDelegateMainParts
    : public content::BrowserMainParts {
 public:
  explicit ContentLifetimeDelegateMainParts(
      ContentLifetimeDelegate* delegate);
  ~ContentLifetimeDelegateMainParts() override;

  // content::BrowserMainParts:
  int PreMainMessageLoopRun() override;
  void WillRunMainMessageLoop(
      std::unique_ptr<base::RunLoop>& run_loop) override;
  void PostMainMessageLoopRun() override;

 private:
  raw_ptr<ContentLifetimeDelegate> delegate_;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_MAIN_PARTS_H_
