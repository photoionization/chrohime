// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LABEL_BUTTON_H_
#define CHROHIME_API_LABEL_BUTTON_H_

#include "chrohime/api/button.h"

namespace views {
class LabelButton;
}

namespace hime {

class Image;

class CHROHIME_EXPORT LabelButton : public Button {
 public:
  LabelButton();

  void SetTitle(const std::u16string& title);
  const std::u16string& GetTitle() const;
  void MakeDefault(bool is_default);
  bool IsDefault() const;

  void SetImage(const scoped_refptr<Image>& image);
  void SetImageForState(const scoped_refptr<Image>& image, Button::State state);

 protected:
  explicit LabelButton(std::unique_ptr<views::LabelButton> to_take);
  ~LabelButton() override;

 private:
  views::LabelButton* GetView() const;
};

}  // namespace hime

#endif  // CHROHIME_API_LABEL_BUTTON_H_
