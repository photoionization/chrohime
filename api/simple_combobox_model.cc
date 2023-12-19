// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/simple_combobox_model.h"

#include <vector>

#include "ui/base/models/combobox_model.h"
#include "ui/base/models/combobox_model_observer.h"

namespace hime {

class SimpleComboboxModelImpl : public ui::ComboboxModel {
 public:
  SimpleComboboxModelImpl() = default;

  void AddItem(std::u16string item) {
    items_.push_back(std::move(item));
    for (auto& observer : observers())
      observer.OnComboboxModelChanged(this);
  }

  void RemoveItemAt(size_t index) {
    if (index < GetItemCount())
      items_.erase(items_.begin() + index);
    for (auto& observer : observers())
      observer.OnComboboxModelChanged(this);
  }

  // ui::ComboboxModel:
  size_t GetItemCount() const override {
    return items_.size();
  }

  std::u16string GetItemAt(size_t index) const override {
    if (index < GetItemCount())
      return items_[index];
    else
      return u"(no data)";
  }

 private:
  std::vector<std::u16string> items_;
};

SimpleComboboxModel::SimpleComboboxModel()
    : ComboboxModel(std::make_unique<SimpleComboboxModelImpl>()) {}

SimpleComboboxModel::~SimpleComboboxModel() = default;

void SimpleComboboxModel::AddItem(std::u16string item) {
  HIME_RETURN_ON_DESTROYED_COMBOBOX_MODEL(this);
  GetModel()->AddItem(std::move(item));
}

void SimpleComboboxModel::RemoveItemAt(size_t index) {
  HIME_RETURN_ON_DESTROYED_COMBOBOX_MODEL(this);
  GetModel()->RemoveItemAt(index);
}

SimpleComboboxModelImpl* SimpleComboboxModel::GetModel() const {
  return static_cast<SimpleComboboxModelImpl*>(model());
}

}  // namespace hime
