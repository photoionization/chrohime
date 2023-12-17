// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/badge.h"

#include "ui/views/controls/badge.h"

namespace hime {

Badge::Badge() : View(std::make_unique<views::Badge>(), LayoutType::kLeaf) {
  UsePreferredSizeForYogaMeasurement();
}

Badge::~Badge() = default;

void Badge::SetText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetText(text);
}

views::Badge* Badge::GetView() const {
  return static_cast<views::Badge*>(view());
}

}  // namespace hime
