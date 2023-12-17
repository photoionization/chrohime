// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/button.h"

#include "ui/views/controls/button/button.h"

namespace hime {

Button::Button(std::unique_ptr<views::Button> to_take)
    : View(std::move(to_take), LayoutType::kLeaf) {
  GetView()->SetCallback(
      base::BindRepeating(&Button::OnClick, base::Unretained(this)));
  UsePreferredSizeForYogaMeasurement();
}

Button::~Button() = default;

views::Button* Button::GetView() const {
  return static_cast<views::Button*>(view());
}

void Button::OnClick() {
  on_click.Emit(this);
}

}  // namespace hime
