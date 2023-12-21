// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CHROHIME_CONTENT_BROWSER_CLIENT_H_
#define CHROHIME_CONTENT_CHROHIME_CONTENT_BROWSER_CLIENT_H_

#include "chrohime/chrohime_export.h"
#include "content/public/browser/content_browser_client.h"

namespace hime {

class ChrohimeContentClient;

class CHROHIME_EXPORT ChrohimeContentBrowserClient
    : public content::ContentBrowserClient {
 public:
  explicit ChrohimeContentBrowserClient(ChrohimeContentClient* content_client);
  ~ChrohimeContentBrowserClient() override;

  // content::ContentBrowserClient:
  std::unique_ptr<content::BrowserMainParts> CreateBrowserMainParts(
      bool is_integration_test) override;

 private:
  raw_ptr<ChrohimeContentClient> content_client_;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CHROHIME_CONTENT_BROWSER_CLIENT_H_
