// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/radio_button.h"

#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/button/radio_button.h"

namespace hime {

namespace {

class RadioButtonImpl : public ViewInputEventDispatcher<hime::RadioButton,
                                                        views::RadioButton> {
 public:
  RadioButtonImpl(hime::RadioButton* delegate,
                  const std::u16string& title,
                  int group_id)
      : ViewBaseDispatcher(delegate, title, group_id) {}
};

}  // namespace

RadioButton::RadioButton(const std::u16string& title, int group_id)
    : Checkbox(std::make_unique<RadioButtonImpl>(this, title, group_id)) {}

RadioButton::~RadioButton() = default;

views::RadioButton* RadioButton::GetView() const {
  return static_cast<views::RadioButton*>(view());
}

}  // namespace hime
