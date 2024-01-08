// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_BITMAP_H_
#define CHROHIME_API_BITMAP_H_

#include "chrohime/api/object.h"
#include "third_party/skia/include/core/SkBitmap.h"

namespace gfx {
class Point;
class Size;
}

namespace hime {

class Buffer;

class CHROHIME_EXPORT Bitmap : public Object {
 public:
  static scoped_refptr<Bitmap> CreateFromJpeg(const Buffer& buffer);
  static scoped_refptr<Bitmap> CreateFromPng(const Buffer& buffer);

  Bitmap();
  explicit Bitmap(SkBitmap bitmap);

  bool IsEmpty() const;
  gfx::Size GetSize() const;
  SkColor GetColor(const gfx::Point& point) const;

  const SkBitmap& sk_bitmap() const { return bitmap_; }

 protected:
  ~Bitmap() override;

 private:
  SkBitmap bitmap_;
};

}  // namespace hime

#endif  // CHROHIME_API_BITMAP_H_
