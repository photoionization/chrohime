// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/label.h"

#include "ui/views/controls/label.h"

namespace hime {

Label::Label()
    : View(std::make_unique<views::Label>(), LayoutType::kContainer),
      label_(static_cast<views::Label*>(view())) {}

Label::~Label() = default;

void Label::SetText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  label_->SetText(text);
}

const std::u16string& Label::GetText() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return label_->GetText();
}

}  // namespace hime
