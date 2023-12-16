// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/material_button.h"

#include "ui/views/controls/button/md_text_button.h"

namespace hime {

MaterialButton::MaterialButton()
    : LabelButton(std::make_unique<views::MdTextButton>()),
      button_(static_cast<views::MdTextButton*>(view())) {}

MaterialButton::~MaterialButton() = default;

void MaterialButton::SetStyle(Style style) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  button_->SetStyle(style);
}

MaterialButton::Style MaterialButton::GetStyle() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, ui::ButtonStyle::kDefault);
  return button_->GetStyle();
}

}  // namespace hime
