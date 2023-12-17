// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/label_button.h"

#include "ui/views/controls/button/label_button.h"

namespace hime {

LabelButton::LabelButton()
    : LabelButton(std::make_unique<views::LabelButton>()) {}

LabelButton::LabelButton(std::unique_ptr<views::LabelButton> to_take)
    : Button(std::move(to_take)),
      button_(static_cast<views::LabelButton*>(view())) {}

LabelButton::~LabelButton() = default;

void LabelButton::SetTitle(const std::u16string& title) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  button_->SetText(title);
}

const std::u16string& LabelButton::GetTitle() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return button_->GetText();
}

void LabelButton::MakeDefault(bool is_default) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  button_->SetIsDefault(is_default);
}

bool LabelButton::IsDefault() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return button_->GetIsDefault();
}

}  // namespace hime
