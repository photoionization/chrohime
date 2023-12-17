// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_RADIO_BUTTON_H_
#define CHROHIME_API_RADIO_BUTTON_H_

#include "chrohime/api/checkbox.h"

namespace views {
class RadioButton;
}

namespace hime {

class CHROHIME_EXPORT RadioButton : public Checkbox {
 public:
  RadioButton(const std::u16string& title, int group_id);

  views::RadioButton* GetView() const;

 protected:
  ~RadioButton() override;
};

}  // namespace hime

#endif  // CHROHIME_API_RADIO_BUTTON_H_
