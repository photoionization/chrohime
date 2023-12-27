// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LIFETIME_H_
#define CHROHIME_API_LIFETIME_H_

#include <memory>

#include "base/memory/weak_ptr.h"
#include "chrohime/api/signal.h"
#include "chrohime/content/content_lifetime_delegate.h"

namespace hime {

class ChrohimeViewsDelegate;
struct LifetimeImpl;

class CHROHIME_EXPORT Lifetime : public ContentLifetimeDelegate {
 public:
  static Lifetime* GetCurrent();

#if BUILDFLAG(IS_WIN)
  Lifetime();
#else
  Lifetime(int argc, const char** argv);
#endif
  virtual ~Lifetime();

  int RunMain();
  void Quit();

  // Events.
  Signal<void()> on_ready;

  base::WeakPtr<Lifetime> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }

 protected:
  // ContentLifetimeDelegate:
#if BUILDFLAG(IS_MAC)
  void OnPreBrowserMain() override;
#endif
  void OnPreMainMessageLoopRun(base::RepeatingClosure quit_closure) override;
  void OnPostMainMessageLoopRun() override;

 private:
#if BUILDFLAG(IS_WIN)
  void Initialize();
#else
  void Initialize(int argc, const char** argv);
#endif
  void Destroy();

  raw_ptr<LifetimeImpl> impl_;
  std::unique_ptr<ChrohimeViewsDelegate> views_delegate_;
  base::RepeatingClosure quit_closure_;
  base::WeakPtrFactory<Lifetime> weak_factory_{this};
};

}  // namespace hime

#endif  // CHROHIME_API_LIFETIME_H_
