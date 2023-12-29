// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/textfield.h"

#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/textfield/textfield.h"

namespace hime {

namespace {

class TextfieldImpl : public ViewEventDispatcher<hime::Textfield,
                                                 views::Textfield> {
 public:
  explicit TextfieldImpl(hime::Textfield* delegate)
      : ViewEventDispatcher(delegate) {}
};

}  // namespace

Textfield::Textfield() : Textfield(std::make_unique<TextfieldImpl>(this)) {}

Textfield::Textfield(std::unique_ptr<views::Textfield> to_take)
    : View(std::move(to_take), LayoutType::kLeaf) {
  UsePreferredSizeForYogaMeasurement();
}

Textfield::~Textfield() = default;

void Textfield::SetType(Type type) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetTextInputType(static_cast<ui::TextInputType>(type));
}

void Textfield::SetText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetText(text);
}

const std::u16string& Textfield::GetText() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return GetView()->GetText();
}

void Textfield::SetPlaceholderText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetPlaceholderText(text);
}

void Textfield::SetReadonly(bool readonly) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetReadOnly(readonly);
}

bool Textfield::IsReadonly() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetReadOnly();
}

void Textfield::SetInvalid(bool invalid) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetInvalid(invalid);
}

bool Textfield::IsInvalid() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetInvalid();
}

views::Textfield* Textfield::GetView() const {
  return static_cast<views::Textfield*>(view());
}

}  // namespace hime
