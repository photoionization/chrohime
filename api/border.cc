// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/border.h"

#include "ui/views/border.h"

namespace hime {

Border::Border(SkColor color, int thickness)
    : Border(views::CreateSolidBorder(thickness, color)) {}

Border::Border(ColorId color_id, int thickness)
    : Border(views::CreateThemedSolidBorder(thickness, color_id)) {}

Border::Border(SkColor color, int thickness, float radius)
    : Border(views::CreateRoundedRectBorder(thickness, radius, color)) {}

Border::Border(ColorId color_id, int thickness, float radius)
    : Border(views::CreateThemedRoundedRectBorder(
          thickness, radius, color_id)) {}

Border::Border(std::unique_ptr<views::Border> to_take)
    : border_(std::move(to_take)) {}

Border::~Border() = default;

std::unique_ptr<views::Border> Border::TransferOwnership() {
  return std::move(border_);
}

}  // namespace hime
