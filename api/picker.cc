// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/picker.h"

#include "chrohime/api/combobox_model.h"
#include "chrohime/api/view_event_dispatcher.h"
#include "ui/base/models/combobox_model.h"
#include "ui/views/controls/combobox/combobox.h"

namespace hime {

namespace {

class PickerImpl : public ViewEventDispatcher<hime::Picker,
                                              views::Combobox> {
 public:
  explicit PickerImpl(hime::Picker* delegate)
      : ViewBaseDispatcher(delegate) {}
};

}  // namespace

Picker::Picker()
    : View(std::make_unique<PickerImpl>(this), LayoutType::kLeaf) {
  GetView()->SetCallback(
      base::BindRepeating(&Picker::OnChange, base::Unretained(this)));
  UsePreferredSizeForYogaMeasurement();
}

Picker::~Picker() = default;

void Picker::SetModel(scoped_refptr<ComboboxModel> model) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetOwnedModel(model->TransferOwnership());
  model_ = std::move(model);
}

ComboboxModel* Picker::GetModel() const {
  return model_.get();
}

void Picker::SelectItemAt(int index) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  if (index < 0)
    GetView()->SetSelectedRow(absl::optional<size_t>());
  else
    GetView()->SetSelectedRow(static_cast<size_t>(index));
}

const std::u16string& Picker::GetSelectedItem() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  absl::optional<size_t> result = GetView()->GetSelectedRow();
  if (!result.has_value())
    return base::EmptyString16();
  return cached_result_ = GetView()->GetTextForRow(result.value());
}

int Picker::GetSelectedItemIndex() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, 0);
  absl::optional<size_t> result = GetView()->GetSelectedRow();
  if (result.has_value())
    return static_cast<int>(result.value());
  else
    return -1;
}

void Picker::OnChange() {
  on_change.Emit(this);
}

views::Combobox* Picker::GetView() const {
  return static_cast<views::Combobox*>(view());
}

}  // namespace hime
