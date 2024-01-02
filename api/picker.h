// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_PICKER_H_
#define CHROHIME_API_PICKER_H_

#include "chrohime/api/view.h"

namespace views {
class Combobox;
}

namespace hime {

class ComboboxModel;

class CHROHIME_EXPORT Picker : public View {
 public:
  Picker();

  void SetModel(scoped_refptr<ComboboxModel> model);
  ComboboxModel* GetModel() const;

  void SelectItemAt(int index);
  const std::u16string& GetSelectedItem() const;
  int GetSelectedItemIndex() const;

  // Events.
  Signal<void(Picker*)> on_change;

 protected:
  ~Picker() override;

 private:
  void OnChange();
  views::Combobox* GetView() const;

  // Cache the result so we can return const string reference in APIs.
  mutable std::u16string cached_result_;

  scoped_refptr<ComboboxModel> model_;
};

}  // namespace hime

#endif  // CHROHIME_API_PICKER_H_
