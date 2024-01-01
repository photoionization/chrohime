// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/toggle_button.h"

#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/button/toggle_button.h"

namespace hime {

namespace {

class ToggleButtonImpl : public ViewEventDispatcher<hime::ToggleButton,
                                                    views::ToggleButton> {
 public:
  explicit ToggleButtonImpl(hime::ToggleButton* delegate)
      : ViewEventDispatcher(delegate) {}
};

}  // namespace

ToggleButton::ToggleButton()
    : Button(std::make_unique<ToggleButtonImpl>(this)) {}

ToggleButton::~ToggleButton() = default;

void ToggleButton::SetOnOff(bool is_on) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetIsOn(is_on);
}

void ToggleButton::SetOnOffAnimated(bool is_on) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->AnimateIsOn(is_on);
}

bool ToggleButton::IsOn() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetIsOn();
}

views::ToggleButton* ToggleButton::GetView() const {
  return static_cast<views::ToggleButton*>(view());
}

}  // namespace hime
