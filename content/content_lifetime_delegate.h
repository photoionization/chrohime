// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CONTENT_LIFETIME_DELEGATE_H_
#define CHROHIME_CONTENT_CONTENT_LIFETIME_DELEGATE_H_

namespace content {
class BrowserContext;
}

namespace hime {

// This header is also used by non-content code so it should only include
// virtual methods without implementations to avoid making //chrohime/content
// a dependency.
class ContentLifetimeDelegate {
 public:
  virtual void OnPreBrowserMain() = 0;
  virtual void OnPreMainMessageLoopRun(
      content::BrowserContext* browser_context) = 0;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CONTENT_LIFETIME_DELEGATE_H_