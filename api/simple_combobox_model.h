// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_SIMPLE_COMBOBOX_MODEL_H_
#define CHROHIME_API_SIMPLE_COMBOBOX_MODEL_H_

#include "chrohime/api/combobox_model.h"

namespace hime {

class SimpleComboboxModelImpl;

class CHROHIME_EXPORT SimpleComboboxModel : public ComboboxModel {
 public:
  SimpleComboboxModel();

  void AddItem(std::u16string item);
  void RemoveItemAt(size_t index);

  SimpleComboboxModelImpl* GetModel() const;

 protected:
  ~SimpleComboboxModel() override;
};

}  // namespace hime

#endif  // CHROHIME_API_SIMPLE_COMBOBOX_MODEL_H_
