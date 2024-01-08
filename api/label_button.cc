// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/label_button.h"

#include "chrohime/api/image.h"
#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/button/label_button.h"

namespace hime {

namespace {

class LabelButtonImpl : public ViewInputEventDispatcher<hime::LabelButton,
                                                        views::LabelButton> {
 public:
  explicit LabelButtonImpl(hime::LabelButton* delegate)
      : ViewBaseDispatcher(delegate) {}
};

}  // namespace

LabelButton::LabelButton()
    : LabelButton(std::make_unique<LabelButtonImpl>(this)) {}

LabelButton::LabelButton(std::unique_ptr<views::LabelButton> to_take)
    : Button(std::move(to_take)) {}

LabelButton::~LabelButton() = default;

void LabelButton::SetTitle(const std::u16string& title) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetText(title);
}

const std::u16string& LabelButton::GetTitle() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return GetView()->GetText();
}

void LabelButton::SetImage(const scoped_refptr<Image>& image) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetImageModel(views::Button::STATE_NORMAL, image->image_model());
}

void LabelButton::SetImageForState(const scoped_refptr<Image>& image,
                                   Button::State state) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetImageModel(static_cast<views::Button::ButtonState>(state),
                           image->image_model());
}

void LabelButton::MakeDefault(bool is_default) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetIsDefault(is_default);
}

bool LabelButton::IsDefault() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return GetView()->GetIsDefault();
}

views::LabelButton* LabelButton::GetView() const {
  return static_cast<views::LabelButton*>(view());
}

}  // namespace hime
