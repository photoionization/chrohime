// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/checkbox.h"

#include "ui/views/controls/button/checkbox.h"

namespace hime {

Checkbox::Checkbox()
    : Checkbox(std::make_unique<views::Checkbox>()) {}

Checkbox::Checkbox(std::unique_ptr<views::Checkbox> to_take)
    : LabelButton(std::move(to_take)) {
  subscription_ = GetView()->AddCheckedChangedCallback(
      base::BindRepeating(&Checkbox::OnChange, base::Unretained(this)));
}

Checkbox::~Checkbox() = default;

void Checkbox::SetChecked(bool checked) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetChecked(checked);
}

bool Checkbox::IsChecked() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetChecked();
}

views::Checkbox* Checkbox::GetView() const {
  return static_cast<views::Checkbox*>(view());
}

void Checkbox::OnChange() {
  on_change.Emit(this);
}

}  // namespace hime
