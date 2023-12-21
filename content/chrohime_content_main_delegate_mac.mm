// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/content/chrohime_content_main_delegate.h"

#include "base/logging.h"
#include "chrohime/content/chrohime_content_client.h"
#include "content/shell/browser/shell_application_mac.h"

namespace hime {

absl::optional<int> ChrohimeContentMainDelegate::PreBrowserMain() {
  absl::optional<int> code = content::ContentMainDelegate::PreBrowserMain();
  if (code.has_value())
    return code;
  // Simply instantiating an instance of ShellCrApplication serves to register
  // it as the application class. Do make sure that no other code has done this
  // first, though.
  CHECK_EQ(NSApp, nil);
  [ShellCrApplication sharedApplication];
  content_client_->OnPreBrowserMain();
  return absl::nullopt;
}

}  // namespace hime
