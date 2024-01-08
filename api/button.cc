// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/button.h"

#include "ui/views/controls/button/button.h"

namespace hime {

static_assert(static_cast<int>(Button::State::kNormal) ==
                  static_cast<int>(views::Button::STATE_NORMAL));
static_assert(static_cast<int>(Button::State::kDisabled) ==
                  static_cast<int>(views::Button::STATE_DISABLED));

Button::Button(std::unique_ptr<views::Button> to_take)
    : View(std::move(to_take), LayoutType::kLeaf) {
  GetView()->SetCallback(
      base::BindRepeating(&Button::OnClick, base::Unretained(this)));
  UsePreferredSizeForYogaMeasurement();
}

Button::~Button() = default;

Button::State Button::GetState() const {
  return static_cast<Button::State>(GetView()->GetState());
}

void Button::OnClick() {
  on_click.Emit(this);
}

views::Button* Button::GetView() const {
  return static_cast<views::Button*>(view());
}

}  // namespace hime
