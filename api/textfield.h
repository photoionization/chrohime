// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_TEXTFIELD_H_
#define CHROHIME_API_TEXTFIELD_H_

#include "chrohime/api/view.h"
#include "ui/base/ime/text_input_type.h"

namespace views {
class Textfield;
}

namespace hime {

class CHROHIME_EXPORT Textfield : public View {
 public:
  enum class Type : int {
    kText = ui::TEXT_INPUT_TYPE_TEXT,
    kPassword = ui::TEXT_INPUT_TYPE_PASSWORD,
    kSearch = ui::TEXT_INPUT_TYPE_SEARCH,
    kNumber = ui::TEXT_INPUT_TYPE_NUMBER,
  };

  Textfield();

  void SetType(Type type);
  void SetText(const std::u16string& text);
  const std::u16string& GetText() const;
  void SetPlaceholderText(const std::u16string& text);
  void SetReadonly(bool readonly);
  bool IsReadonly() const;
  void SetInvalid(bool invalid);
  bool IsInvalid() const;

  views::Textfield* GetView() const;

 protected:
  explicit Textfield(std::unique_ptr<views::Textfield> to_take);
  ~Textfield() override;
};

}  // namespace hime

#endif  // CHROHIME_API_TEXTFIELD_H_
