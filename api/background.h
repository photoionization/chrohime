// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_BACKGROUND_H_
#define CHROHIME_API_BACKGROUND_H_

#include "chrohime/api/color_id.h"
#include "chrohime/api/object.h"
#include "third_party/skia/include/core/SkColor.h"

namespace views {
class Background;
}

namespace hime {

class CHROHIME_EXPORT Background : public Object {
 public:
  explicit Background(SkColor color);
  explicit Background(ColorId color_id);
  Background(SkColor color, int border, float radius);
  Background(ColorId color_id, int border, float radius);

  // Internal: Give the ownership of underlying views::Background to caller.
  std::unique_ptr<views::Background> TransferOwnership();

 protected:
  explicit Background(std::unique_ptr<views::Background> to_take);
  ~Background() override;

  // Due to restrictions of ui/views API, the views::Background object can not
  // be shared between views and we must handle it ownership when setting the
  // background of a view.
  std::unique_ptr<views::Background> background_;
};

}  // namespace hime

#endif  // CHROHIME_API_BACKGROUND_H_
