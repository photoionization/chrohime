// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/background.h"

#include "ui/views/background.h"

namespace hime {

Background::Background(SkColor color)
    : Background(views::CreateSolidBackground(color)) {}

Background::Background(SkColor color, int border, float radius)
    : Background(views::CreateRoundedRectBackground(color, radius, border)) {}

Background::Background(std::unique_ptr<views::Background> to_take)
    : background_(std::move(to_take)) {}

Background::~Background() = default;

std::unique_ptr<views::Background> Background::TransferOwnership() {
  return std::move(background_);
}

}  // namespace hime
