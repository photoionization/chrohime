// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_COMBOBOX_H_
#define CHROHIME_API_COMBOBOX_H_

#include "chrohime/api/view.h"

namespace views {
class EditableCombobox;
}

namespace hime {

class ComboboxModel;

class CHROHIME_EXPORT Combobox : public View {
 public:
  Combobox();

  void SetModel(scoped_refptr<ComboboxModel> model);
  ComboboxModel* GetModel() const;

  void SetText(const std::u16string& text);
  const std::u16string& GetText() const;
  void SetPlaceholderText(const std::u16string& text);

  // Events.
  Signal<void(Combobox*)> on_change;

 protected:
  ~Combobox() override;

 private:
  void OnChange();

  views::EditableCombobox* GetView() const;

  scoped_refptr<ComboboxModel> model_;
};

}  // namespace hime

#endif  // CHROHIME_API_COMBOBOX_H_
