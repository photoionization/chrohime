// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_IMAGE_H_
#define CHROHIME_API_IMAGE_H_

#include "chrohime/api/object.h"
#include "ui/base/models/image_model.h"

namespace base {
class FilePath;
}

namespace hime {

class SkiaImage;

class CHROHIME_EXPORT Image : public Object {
 public:
  static scoped_refptr<Image> CreateFromFilePath(const base::FilePath& path);

  Image();
  explicit Image(const scoped_refptr<SkiaImage>& skia_image);

  bool IsEmpty() const;
  gfx::Size GetSize() const;

  const ui::ImageModel& image_model() const { return model_; }

 protected:
  ~Image() override;

 private:
  ui::ImageModel model_;
};

}  // namespace hime

#endif  // CHROHIME_API_IMAGE_H_
