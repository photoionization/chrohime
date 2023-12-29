// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_BORDER_H_
#define CHROHIME_API_BORDER_H_

#include "chrohime/api/color_id.h"
#include "chrohime/api/object.h"
#include "third_party/skia/include/core/SkColor.h"

namespace views {
class Border;
}

namespace hime {

class CHROHIME_EXPORT Border : public Object {
 public:
  Border(SkColor color, int thickness);
  Border(ColorId color_id, int thickness);
  Border(SkColor color, int thickness, float radius);
  Border(ColorId color_id, int thickness, float radius);

  // Internal: Give the ownership of underlying views::Border to caller.
  std::unique_ptr<views::Border> TransferOwnership();

 protected:
  explicit Border(std::unique_ptr<views::Border> to_take);
  ~Border() override;

 private:
  // Due to restrictions of ui/views API, the views::Border object can not
  // be shared between views and we must handle it ownership when setting the
  // background of a view.
  std::unique_ptr<views::Border> border_;
};

}  // namespace hime

#endif  // CHROHIME_API_BORDER_H_
