// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_CHECKBOX_H_
#define CHROHIME_API_CHECKBOX_H_

#include "base/callback_list.h"
#include "chrohime/api/label_button.h"

namespace views {
class Checkbox;
}

namespace hime {

class CHROHIME_EXPORT Checkbox : public LabelButton {
 public:
  Checkbox();

  void SetChecked(bool checked);
  bool IsChecked() const;

  // Events.
  Signal<void(Checkbox*)> on_change;

 protected:
  explicit Checkbox(std::unique_ptr<views::Checkbox> to_take);
  ~Checkbox() override;

  void OnChange();

 private:
  views::Checkbox* GetView() const;

  base::CallbackListSubscription subscription_;
};

}  // namespace hime

#endif  // CHROHIME_API_CHECKBOX_H_
