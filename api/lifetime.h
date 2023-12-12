// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LIFETIME_H_
#define CHROHIME_API_LIFETIME_H_

#include <memory>

#include "base/memory/weak_ptr.h"
#include "chrohime/api/signal.h"

namespace hime {

struct LifetimeImpl;

class CHROHIME_EXPORT Lifetime {
 public:
  static Lifetime* GetCurrent();

#if BUILDFLAG(IS_WIN)
  Lifetime();
#else
  Lifetime(int argc, const char** argv);
#endif
  ~Lifetime();

  int RunMain();

  // Events.
  Signal<void()> on_ready;

  base::WeakPtr<Lifetime> GetWeakPtr() { return weak_factory_.GetWeakPtr(); }

 private:
#if BUILDFLAG(IS_WIN)
  void Initialize();
#else
  void Initialize(int argc, const char** argv);
#endif
  void Destroy();

  raw_ptr<LifetimeImpl> impl_;
  base::WeakPtrFactory<Lifetime> weak_factory_{this};
};

}  // namespace hime

#endif  // CHROHIME_API_LIFETIME_H_
