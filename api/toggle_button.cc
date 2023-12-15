// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/toggle_button.h"

#include "ui/views/controls/button/toggle_button.h"

namespace hime {

ToggleButton::ToggleButton()
    : Button(std::make_unique<views::ToggleButton>()),
      button_(static_cast<views::ToggleButton*>(view())) {
  // ToggleButton has fixed size.
  gfx::Size preferred_size = GetPreferredSize();
  SetNumberStyle(u"width", preferred_size.width());
  SetNumberStyle(u"height", preferred_size.height());
}

ToggleButton::~ToggleButton() = default;

void ToggleButton::SetOnOff(bool is_on) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  button_->SetIsOn(is_on);
}

void ToggleButton::SetOnOffAnimated(bool is_on) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  button_->AnimateIsOn(is_on);
}

bool ToggleButton::IsOn() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return button_->GetIsOn();
}

}  // namespace hime
