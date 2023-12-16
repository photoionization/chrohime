// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/checkbox.h"

#include "ui/views/controls/button/checkbox.h"

namespace hime {

Checkbox::Checkbox()
    : Checkbox(std::make_unique<views::Checkbox>()) {}

Checkbox::Checkbox(std::unique_ptr<views::Checkbox> to_take)
    : LabelButton(std::move(to_take)),
      checkbox_(static_cast<views::Checkbox*>(view())) {
  subscription_ = checkbox_->AddCheckedChangedCallback(
      base::BindRepeating(&Checkbox::OnChange, base::Unretained(this)));
}

Checkbox::~Checkbox() = default;

void Checkbox::SetChecked(bool checked) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  checkbox_->SetChecked(checked);
}

bool Checkbox::IsChecked() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return checkbox_->GetChecked();
}

void Checkbox::OnChange() {
  on_change.Emit(this);
}

}  // namespace hime
