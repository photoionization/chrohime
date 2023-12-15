// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_BUTTON_H_
#define CHROHIME_API_BUTTON_H_

#include "chrohime/api/view.h"

namespace views {
class Button;
}

namespace hime {

class CHROHIME_EXPORT Button : public View {
 public:

 protected:
  explicit Button(std::unique_ptr<views::Button> to_take);
  ~Button() override;

  raw_ptr<views::Button> button_;
};

}  // namespace hime

#endif  // CHROHIME_API_BUTTON_H_
