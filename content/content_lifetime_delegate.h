// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CONTENT_LIFETIME_DELEGATE_H_
#define CHROHIME_CONTENT_CONTENT_LIFETIME_DELEGATE_H_

#include <memory>

namespace base {
class RunLoop;
}

namespace hime {

// This header is also used by non-content code so it should only include
// virtual methods without implementations to avoid making //chrohime/content
// a dependency.
class ContentLifetimeDelegate {
 public:
  virtual ~ContentLifetimeDelegate() {}

  virtual void OnPreBrowserMain() {}
  virtual void OnPreMainMessageLoopRun() {}
  virtual void OnWillRunMainMessageLoop(
      std::unique_ptr<base::RunLoop>& run_loop) {}
  virtual void OnPostMainMessageLoopRun() {}
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CONTENT_LIFETIME_DELEGATE_H_
