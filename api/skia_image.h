// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_SKIA_IMAGE_H_
#define CHROHIME_API_SKIA_IMAGE_H_

#include "chrohime/api/object.h"
#include "ui/gfx/image/image_skia.h"

namespace base {
class FilePath;
}

namespace hime {

class Bitmap;

class CHROHIME_EXPORT SkiaImage : public Object {
 public:
  static scoped_refptr<SkiaImage> CreateFromFilePath(
      const base::FilePath& path);

  SkiaImage();
  SkiaImage(const scoped_refptr<Bitmap>& bitmap, float scale);

  bool IsEmpty() const;
  gfx::Size GetSize() const;

  void AddBitmapRepresentation(const scoped_refptr<Bitmap>& bitmap,
                               float scale);
  void RemoveRepresentation(float scale);
  bool HasRepresentation(float scale) const;

  const gfx::ImageSkia& image_skia() const { return image_; }

 private:
  ~SkiaImage() override;

  gfx::ImageSkia image_;
};

}  // namespace hime

#endif  // CHROHIME_API_SKIA_IMAGE_H_
