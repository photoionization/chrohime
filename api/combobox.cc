// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/combobox.h"

#include "chrohime/api/combobox_model.h"
#include "chrohime/api/view_event_dispatcher.h"
#include "ui/base/models/combobox_model.h"
#include "ui/views/controls/editable_combobox/editable_combobox.h"

namespace hime {

namespace {

class ComboboxImpl : public ViewEventDispatcher<hime::Combobox,
                                                views::EditableCombobox> {
 public:
  explicit ComboboxImpl(hime::Combobox* delegate)
      : ViewBaseDispatcher(delegate) {}
};

}  // namespace

Combobox::Combobox()
    : View(std::make_unique<ComboboxImpl>(this), LayoutType::kLeaf) {
  GetView()->SetCallback(
      base::BindRepeating(&Combobox::OnChange, base::Unretained(this)));
  UsePreferredSizeForYogaMeasurement();
}

Combobox::~Combobox() = default;

void Combobox::SetModel(scoped_refptr<ComboboxModel> model) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetModel(model->TransferOwnership());
  model_ = std::move(model);
}

ComboboxModel* Combobox::GetModel() const {
  return model_.get();
}

void Combobox::SetText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetText(text);
}

const std::u16string& Combobox::GetText() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return GetView()->GetText();
}

void Combobox::SetPlaceholderText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetPlaceholderText(text);
}

views::EditableCombobox* Combobox::GetView() const {
  return static_cast<views::EditableCombobox*>(view());
}

void Combobox::OnChange() {
  on_change.Emit(this);
}

}  // namespace hime
