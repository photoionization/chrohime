// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/slider.h"

#include "chrohime/api/view_event_dispatcher.h"
#include "yoga/Yoga.h"

namespace views::test {

// Slider only allows setting listener via constructor, bypass the restriction
// by pretending to be its friend.
class SliderTestApi {
 public:
  SliderTestApi(views::Slider* slider, hime::Slider* listener) {
    slider->set_listener(listener);
  }
};

}  // namespace views::test

namespace hime {

namespace {

class SliderImpl : public ViewEventDispatcher<hime::Slider, views::Slider> {
 public:
  explicit SliderImpl(hime::Slider* delegate) : ViewEventDispatcher(delegate) {}
};

YGSize MeasureSlider(YGNodeConstRef node,
                     float width, YGMeasureMode mode,
                     float height, YGMeasureMode height_mode) {
  auto* view = static_cast<View*>(YGNodeGetContext(node));
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(view, YGSize(0, 0));
  // Slider has a fixed height and flexible width.
  gfx::Size size = view->view()->GetPreferredSize();
  if (mode == YGMeasureModeExactly)
    width = size.width();
  return {width, static_cast<float>(size.height())};
}

}  // namespace

Slider::Slider()
    : View(std::make_unique<SliderImpl>(this), LayoutType::kLeaf) {
  views::test::SliderTestApi set_listener(GetView(), this);
  YGNodeSetMeasureFunc(yoga_node(), MeasureSlider);
}

Slider::~Slider() = default;

void Slider::SetValue(float value) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetValue(value);
}

float Slider::GetValue() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, 0);
  return GetView()->GetValue();
}

views::Slider* Slider::GetView() const {
  return static_cast<views::Slider*>(view());
}

void Slider::SliderValueChanged(views::Slider* sender,
                                float value,
                                float old_value,
                                views::SliderChangeReason reason) {
  if (reason == views::SliderChangeReason::kByApi)
    return;
  on_change.Emit(this, value, old_value);
}

}  // namespace hime
