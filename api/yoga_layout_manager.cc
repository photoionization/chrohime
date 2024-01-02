// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/yoga_layout_manager.h"

#include <limits>

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
      child->SetBounds(
          gfx::Rect(YGNodeLayoutGetLeft(node), YGNodeLayoutGetTop(node),
                    YGNodeLayoutGetWidth(node), YGNodeLayoutGetHeight(node)));
    }
  }
}

gfx::Size YogaLayoutManager::GetPreferredSize(const views::View*) const {
  // This method is called by views that does not implement the
  // CalculatePreferredSize method, in chrohime the only case is the container
  // type of views.
  DCHECK_EQ(host_->layout_type(), View::LayoutType::kContainer);
  float nan = std::numeric_limits<float>::quiet_NaN();
  YGNodeCalculateLayout(host_->yoga_node(), nan, nan, YGDirectionLTR);
  return gfx::Size(YGNodeLayoutGetWidth(host_->yoga_node()),
                   YGNodeLayoutGetHeight(host_->yoga_node()));
}

}  // namespace hime
