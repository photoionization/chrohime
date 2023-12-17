// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/progress_bar.h"

#include <algorithm>

#include "ui/views/controls/progress_bar.h"
#include "yoga/Yoga.h"

namespace hime {

namespace {

YGSize MeasureProgressBar(YGNodeConstRef node,
                          float width, YGMeasureMode mode,
                          float height, YGMeasureMode height_mode) {
  // The ProgressBar just grows to take the full space, usually users don't want
  // it to take full height, but we have a safe default height to ensure a
  // proper default size.
  return {width, height};
}

}  // namespace

ProgressBar::ProgressBar()
    : View(std::make_unique<views::ProgressBar>(), LayoutType::kLeaf) {
  YGNodeSetMeasureFunc(yoga_node(), MeasureProgressBar);
  // Give ProgressBar a proper default height.
  SetStyleNumber(u"height", 5);
}

ProgressBar::~ProgressBar() = default;

void ProgressBar::SetValue(float value) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetValue(static_cast<double>(std::clamp(value, 0.f, 1.f)));
}

float ProgressBar::GetValue() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, 0);
  return static_cast<float>(GetView()->GetValue());
}

void ProgressBar::SetIndeterminate(bool indeterminate) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetValue(-1);
}

bool ProgressBar::IsIndeterminate() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetValue() < 0;
}

views::ProgressBar* ProgressBar::GetView() const {
  return static_cast<views::ProgressBar*>(view());
}

}  // namespace hime
