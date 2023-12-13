// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/root_view.h"

#include "chrohime/api/view.h"
#include "chrohime/api/window.h"
#include "ui/views/background.h"

namespace hime {

RootView::RootView(Window* window) : window_(window) {
  set_owned_by_client();
#if !BUILDFLAG(IS_MAC)
  SetBackground(views::CreateSolidBackground(SK_ColorWHITE));
#endif
}

RootView::~RootView() = default;

void RootView::Layout() {
  if (!window_->GetContentView())
    return;
  window_->GetContentView()->view()->SetBoundsRect(gfx::Rect(size()));
}

}  // namespace hime
