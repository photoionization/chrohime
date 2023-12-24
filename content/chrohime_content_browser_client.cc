// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/content/chrohime_content_browser_client.h"

#include "chrohime/content/chrohime_content_main_main_parts.h"

namespace hime {

ChrohimeContentBrowserClient::ChrohimeContentBrowserClient(
    ContentLifetimeDelegate* delegate) : delegate_(delegate) {}

ChrohimeContentBrowserClient::~ChrohimeContentBrowserClient() = default;

std::unique_ptr<content::BrowserMainParts>
ChrohimeContentBrowserClient::CreateBrowserMainParts(bool is_integration_test) {
  return std::make_unique<ContentLifetimeDelegateMainParts>(
      delegate_.get());
}

}  // namespace hime
