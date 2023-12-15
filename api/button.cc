// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/button.h"

#include "ui/views/controls/button/button.h"

namespace hime {

Button::Button(std::unique_ptr<views::Button> to_take)
    : View(std::move(to_take), LayoutType::kLeaf),
      button_(static_cast<views::Button*>(view())) {}

Button::~Button() = default;

}  // namespace hime
