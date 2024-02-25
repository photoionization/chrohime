// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_DELEGATE_H_
#define CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_DELEGATE_H_

#include "chrohime/content/chrohime_content_client.h"
#include "content/public/app/content_main_delegate.h"

namespace hime {

class ChrohimeContentBrowserClient;
class ContentLifetimeDelegate;

class CHROHIME_EXPORT ChrohimeContentMainDelegate
    : public content::ContentMainDelegate {
 public:
  explicit ChrohimeContentMainDelegate(ContentLifetimeDelegate* delegate);
  ~ChrohimeContentMainDelegate() override;

  // content::ContentMainDelegate:
  std::optional<int> BasicStartupComplete() override;
  void PreSandboxStartup() override;
#if BUILDFLAG(IS_MAC)
  std::optional<int> PreBrowserMain() override;
#endif
  content::ContentClient* CreateContentClient() override;
  content::ContentBrowserClient* CreateContentBrowserClient() override;

 private:
  raw_ptr<ContentLifetimeDelegate> delegate_;
  std::unique_ptr<ChrohimeContentBrowserClient> browser_client_;
  ChrohimeContentClient content_client_;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_DELEGATE_H_
