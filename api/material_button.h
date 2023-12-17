// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_MATERIAL_BUTTON_H_
#define CHROHIME_API_MATERIAL_BUTTON_H_

#include "chrohime/api/label_button.h"
#include "ui/base/ui_base_types.h"

namespace views {
class MdTextButton;
}

namespace hime {

class CHROHIME_EXPORT MaterialButton : public LabelButton {
 public:
  using Style = ui::ButtonStyle;

  MaterialButton();

  void SetStyle(Style style);
  Style GetStyle() const;

  views::MdTextButton* GetView() const;

 protected:
  ~MaterialButton() override;
};

}  // namespace hime

#endif  // CHROHIME_API_MATERIAL_BUTTON_H_
