// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/root_view.h"

#include "chrohime/api/view.h"
#include "chrohime/api/window.h"
#include "ui/views/background.h"

namespace hime {

RootView::RootView() {
  set_owned_by_client();
#if !BUILDFLAG(IS_MAC)
  SetBackground(views::CreateSolidBackground(SK_ColorWHITE));
#endif
}

RootView::~RootView() = default;

void RootView::SetContentView(std::unique_ptr<views::View> view) {
  if (content_view_)
    RemoveChildView(content_view_);
  content_view_ = view.get();
  AddChildView(std::move(view));
}

void RootView::Layout() {
  if (content_view_)
    content_view_->SetBoundsRect(gfx::Rect(size()));
}

}  // namespace hime
