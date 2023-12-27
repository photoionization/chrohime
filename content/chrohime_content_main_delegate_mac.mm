// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/content/chrohime_content_main_delegate.h"

#include "chrohime/content/content_lifetime_delegate.h"

namespace hime {

absl::optional<int> ChrohimeContentMainDelegate::PreBrowserMain() {
  absl::optional<int> code = content::ContentMainDelegate::PreBrowserMain();
  if (code.has_value())
    return code;
  delegate_->OnPreBrowserMain();
  return absl::nullopt;
}

}  // namespace hime
