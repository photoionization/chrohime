// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_BADGE_H_
#define CHROHIME_API_BADGE_H_

#include "chrohime/api/view.h"

namespace views {
class Badge;
}

namespace hime {

class CHROHIME_EXPORT Badge : public View {
 public:
  Badge();

  void SetText(const std::u16string& text);
  // TODO(zcbenz): Add GetText() API once after following CL is merged.
  // https://chromium-review.googlesource.com/c/chromium/src/+/4226253
  // const std::u16string& GetText() const;

 protected:
  ~Badge() override;

 private:
  views::Badge* GetView() const;
};

}  // namespace hime

#endif  // CHROHIME_API_BADGE_H_
