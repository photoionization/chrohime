// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime.h"

#include <iostream>

#include "base/check.h"
#include "base/command_line.h"

namespace hime {

namespace {

thread_local Lifetime* g_lifetime = nullptr;

}  // namespace

// static
Lifetime* Lifetime::GetCurrent() {
  return g_lifetime;
}

#if BUILDFLAG(IS_WIN)
Lifetime::Lifetime() {
  bool success = base::CommandLine::Init(0, nullptr);
#else
Lifetime::Lifetime(int argc, const char** argv) {
  bool success = base::CommandLine::Init(argc, argv);
#endif
  CHECK(success) << "Failed to initialize CommandLine, has Lifetime been "
                    "created twice, or is the code running in Chromium?";
  g_lifetime = this;

#if BUILDFLAG(IS_WIN)
  Initialize();
#else
  Initialize(argc, argv);
#endif
}

Lifetime::~Lifetime() {
  g_lifetime = nullptr;
  Destroy();
}

}  // namespace hime
