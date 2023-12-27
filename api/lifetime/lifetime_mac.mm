// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime.h"

#include "chrohime/api/lifetime/chrohime_app.h"
#include "chrohime/api/lifetime/chrohime_app_delegate.h"

namespace hime {

namespace {

ChrohimeAppDelegate* __strong g_app_delegate;

}  // namespace

void Lifetime::OnPreBrowserMain() {
  CHECK_EQ(NSApp, nil);
  ChrohimeApplication* app = [ChrohimeApplication sharedApplication];
  g_app_delegate = [[ChrohimeAppDelegate alloc] initWithShell:this];
  app.delegate = g_app_delegate;
}

}  // namespace hime
