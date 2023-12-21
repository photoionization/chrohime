// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_DELEGATE_H_
#define CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_DELEGATE_H_

#include "chrohime/chrohime_export.h"
#include "content/public/app/content_main_delegate.h"
#include "content/shell/common/shell_content_client.h"

namespace hime {

class ChrohimeContentBrowserClient;
class ChrohimeContentClient;

class CHROHIME_EXPORT ChrohimeContentMainDelegate
    : public content::ContentMainDelegate {
 public:
  explicit ChrohimeContentMainDelegate(ChrohimeContentClient* content_client);
  ~ChrohimeContentMainDelegate() override;

  // content::ContentMainDelegate:
  absl::optional<int> BasicStartupComplete() override;
  void PreSandboxStartup() override;
#if BUILDFLAG(IS_MAC)
  absl::optional<int> PreBrowserMain() override;
#endif
  content::ContentClient* CreateContentClient() override;
  content::ContentBrowserClient* CreateContentBrowserClient() override;

 private:
  raw_ptr<ChrohimeContentClient> content_client_;
  std::unique_ptr<ChrohimeContentBrowserClient> browser_client_;
  content::ShellContentClient shell_content_client_;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CHROHIME_CONTENT_MAIN_DELEGATE_H_
