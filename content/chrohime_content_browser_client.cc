// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/content/chrohime_content_browser_client.h"

#include "chrohime/content/chrohime_content_main_main_parts.h"

namespace hime {

ChrohimeContentBrowserClient::ChrohimeContentBrowserClient(
    ChrohimeContentClient* content_client) : content_client_(content_client) {}

ChrohimeContentBrowserClient::~ChrohimeContentBrowserClient() = default;

std::unique_ptr<content::BrowserMainParts>
ChrohimeContentBrowserClient::CreateBrowserMainParts(bool is_integration_test) {
  return std::make_unique<ChrohimeContentClientMainParts>(
      content_client_.get());
}

}  // namespace hime
