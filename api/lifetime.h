// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LIFETIME_H_
#define CHROHIME_API_LIFETIME_H_

#include "base/memory/weak_ptr.h"
#include "chrohime/api/signal.h"
#include "chrohime/content/content_lifetime_delegate.h"
#include "ui/views/buildflags.h"

namespace display {
class Screen;
#if BUILDFLAG(IS_MAC)
class ScopedNativeScreen;
#endif
}

namespace wm {
class WMState;
}

namespace hime {

class ChrohimeViewsDelegate;

class CHROHIME_EXPORT Lifetime : public ContentLifetimeDelegate {
 public:
  static Lifetime* GetCurrent();

#if BUILDFLAG(IS_WIN)
  Lifetime();
#else
  Lifetime(int argc, const char** argv);
#endif
  ~Lifetime() override;

  int RunMain();
  void Quit();

  ChrohimeViewsDelegate* views_delegate() const {
    return views_delegate_.get();
  }

  // Events.
  Signal<void()> on_ready;

  base::WeakPtr<Lifetime> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }

 protected:
  // ContentLifetimeDelegate:
#if BUILDFLAG(IS_MAC)
  void OnPreBrowserMain() override;
#endif
  void OnPreMainMessageLoopRun() override;
  void OnWillRunMainMessageLoop(
      std::unique_ptr<base::RunLoop>& run_loop) override;
  void OnPostMainMessageLoopRun() override;

 private:
#if BUILDFLAG(IS_WIN)
  void Initialize();
#else
  void Initialize(int argc, const char** argv);
#endif
  void Destroy();

  std::unique_ptr<base::RunLoop> run_loop_;
#if BUILDFLAG(IS_MAC)
  std::unique_ptr<display::ScopedNativeScreen> desktop_screen_;
#endif
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  std::unique_ptr<wm::WMState> wm_state_;
  std::unique_ptr<display::Screen> screen_;
#endif
  std::unique_ptr<ChrohimeViewsDelegate> views_delegate_;

  raw_ptr<ContentLifetimeDelegate> impl_;
  base::RepeatingClosure quit_closure_;
  base::WeakPtrFactory<Lifetime> weak_factory_{this};
};

}  // namespace hime

#endif  // CHROHIME_API_LIFETIME_H_
