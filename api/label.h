// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LABEL_H_
#define CHROHIME_API_LABEL_H_

#include "chrohime/api/view.h"

namespace views {
class Label;
}

namespace hime {

class CHROHIME_EXPORT Label : public View {
 public:
  Label();

  void SetText(const std::u16string& text);
  const std::u16string& GetText() const;
  void SetMultiLine(bool multi_line);
  bool IsMultiLine() const;
  bool SetSelectable(bool selectable);
  bool IsSelectable() const;

 protected:
  explicit Label(std::unique_ptr<views::Label> to_take);
  ~Label() override;

 private:
  views::Label* GetView() const;
};

}  // namespace hime

#endif  // CHROHIME_API_LABEL_H_
