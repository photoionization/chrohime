// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/radio_button.h"

#include "ui/views/controls/button/radio_button.h"

namespace hime {

RadioButton::RadioButton(const std::u16string& title, int group_id)
    : Checkbox(std::make_unique<views::RadioButton>(title, group_id)),
      radio_(static_cast<views::RadioButton*>(view())) {}

RadioButton::~RadioButton() = default;

}  // namespace hime
