// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_TOGGLE_BUTTON_H_
#define CHROHIME_API_TOGGLE_BUTTON_H_

#include "chrohime/api/button.h"

namespace views {
class ToggleButton;
}

namespace hime {

class CHROHIME_EXPORT ToggleButton : public Button {
 public:
  ToggleButton();

  void SetOnOff(bool is_on);
  void SetOnOffAnimated(bool is_on);
  bool IsOn() const;

  views::ToggleButton* GetView() const;

 protected:
  ~ToggleButton() override;
};

}  // namespace hime

#endif  // CHROHIME_API_TOGGLE_BUTTON_H_
