// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/content/chrohime_content_main_main_parts.h"

#include "chrohime/content/content_lifetime_delegate.h"
#include "content/public/common/result_codes.h"

namespace hime {

ContentLifetimeDelegateMainParts::ContentLifetimeDelegateMainParts(
    ContentLifetimeDelegate* delegate) : delegate_(delegate) {}

ContentLifetimeDelegateMainParts::~ContentLifetimeDelegateMainParts() = default;

int ContentLifetimeDelegateMainParts::PreMainMessageLoopRun() {
  delegate_->OnPreMainMessageLoopRun();
  return content::RESULT_CODE_NORMAL_EXIT;
}

void ContentLifetimeDelegateMainParts::WillRunMainMessageLoop(
    std::unique_ptr<base::RunLoop>& run_loop) {
  delegate_->OnWillRunMainMessageLoop(run_loop);
}

void ContentLifetimeDelegateMainParts::PostMainMessageLoopRun() {
  delegate_->OnPostMainMessageLoopRun();
}

}  // namespace hime
