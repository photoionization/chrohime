// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/label.h"

#include "ui/views/controls/label.h"

namespace hime {

Label::Label() : Label(std::make_unique<views::Label>()) {}

Label::Label(std::unique_ptr<views::Label> to_take)
    : View(std::move(to_take), LayoutType::kLeaf) {
  UsePreferredSizeForYogaMeasurement();
}

Label::~Label() = default;

void Label::SetText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetText(text);
}

const std::u16string& Label::GetText() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return GetView()->GetText();
}

void Label::SetMultiLine(bool multi_line) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetMultiLine(multi_line);
}

bool Label::IsMultiLine() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetMultiLine();
}

bool Label::SetSelectable(bool selectable) {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->SetSelectable(selectable);
}

bool Label::IsSelectable() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetSelectable();
}

views::Label* Label::GetView() const {
  return static_cast<views::Label*>(view());
}

}  // namespace hime
