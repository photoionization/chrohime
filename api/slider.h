// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_SLIDER_H_
#define CHROHIME_API_SLIDER_H_

#include "chrohime/api/view.h"
#include "ui/views/controls/slider.h"

namespace hime {

class CHROHIME_EXPORT Slider : public View,
                               public views::SliderListener {
 public:
  Slider();

  void SetValue(float value);
  float GetValue() const;

  // Events.
  Signal<void(Slider*, float new_value, float old_value)> on_change;

 protected:
  ~Slider() override;

  // views::SliderListener:
  void SliderValueChanged(views::Slider* sender,
                          float value,
                          float old_value,
                          views::SliderChangeReason reason) override;

 private:
  views::Slider* GetView() const;
};

}  // namespace hime

#endif  // CHROHIME_API_SLIDER_H_
