// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/combobox_model.h"

#include "base/strings/string_util.h"
#include "ui/base/models/combobox_model.h"

namespace hime {

ComboboxModel::ComboboxModel(std::unique_ptr<ui::ComboboxModel> to_take)
    : model_(to_take.get()),
      ownership_(std::move(to_take)) {
  model_->AddObserver(this);
}

ComboboxModel::~ComboboxModel() {
  if (model_) {
    // Unsubscribe so OnComboboxModelDestroying will not be called for
    // self-destruction.
    model_->RemoveObserver(this);
  }
}

size_t ComboboxModel::GetItemCount() const {
  HIME_RETURN_VALUE_ON_DESTROYED_COMBOBOX_MODEL(this, 0);
  return model_->GetItemCount();
}

std::u16string ComboboxModel::GetItemText(size_t index) const {
  HIME_RETURN_VALUE_ON_DESTROYED_COMBOBOX_MODEL(this, base::EmptyString16());
  return model_->GetItemAt(index);
}

std::unique_ptr<ui::ComboboxModel> ComboboxModel::TransferOwnership() {
  return std::move(ownership_);
}

void ComboboxModel::OnComboboxModelChanged(ui::ComboboxModel* model) {
}

void ComboboxModel::OnComboboxModelDestroying(ui::ComboboxModel* model) {
  // This method is called when the ownership has been transferred to other
  // classes and then gets deleted there.
  DCHECK(!ownership_);
  model_ = nullptr;
}

}  // namespace hime
