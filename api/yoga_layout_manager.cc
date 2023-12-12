// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/yoga_layout_manager.h"

#include "chrohime/api/view.h"
#include "ui/views/view.h"
#include "yoga/Yoga.h"

namespace hime {

YogaLayoutManager::YogaLayoutManager(View* host) : host_(host) {}

YogaLayoutManager::~YogaLayoutManager() = default;

void YogaLayoutManager::Layout(views::View* view) {
  if (!view->GetVisible())
    return;
  if (host_->IsRootYogaNode()) {
    // For root CSS node, calculate the layout before setting bounds.
    gfx::Size size = view->size();
    YGNodeCalculateLayout(host_->yoga_node(),
                          size.width(), size.height(),
                          YGDirectionLTR);
  }
  for (size_t i = 0; i < host_->ChildCount(); ++i) {
    View* child = host_->ChildAt(i);
    if (child->IsVisible()) {
      YGNodeRef node = child->yoga_node();
      child->view()->SetBounds(
          YGNodeLayoutGetLeft(node), YGNodeLayoutGetTop(node),
          YGNodeLayoutGetWidth(node), YGNodeLayoutGetHeight(node));
    }
  }
}

gfx::Size YogaLayoutManager::GetPreferredSize(const views::View*) const {
  return gfx::Size();
}

}  // namespace hime
