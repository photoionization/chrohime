// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_BUTTON_H_
#define CHROHIME_API_BUTTON_H_

#include "chrohime/api/signal.h"
#include "chrohime/api/view.h"

namespace ui {
class Event;
}

namespace views {
class Button;
}

namespace hime {

class CHROHIME_EXPORT Button : public View {
 public:
  // Events.
  Signal<void(Button*)> on_click;

 protected:
  explicit Button(std::unique_ptr<views::Button> to_take);
  ~Button() override;

 private:
  void OnClick(const ui::Event& event);

  raw_ptr<views::Button> button_;
};

}  // namespace hime

#endif  // CHROHIME_API_BUTTON_H_
