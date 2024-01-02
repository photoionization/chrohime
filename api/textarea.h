// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_TEXTAREA_H_
#define CHROHIME_API_TEXTAREA_H_

#include "chrohime/api/textfield.h"

namespace views {
class Textarea;
}

namespace hime {

class CHROHIME_EXPORT Textarea : public Textfield {
 public:
  Textarea();

 protected:
  ~Textarea() override;

 private:
  views::Textarea* GetView() const;
};

}  // namespace hime

#endif  // CHROHIME_API_TEXTAREA_H_
