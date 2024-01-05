// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/skia_image.h"

#include "chrohime/api/bitmap.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/image/image_skia_rep.h"

namespace hime {

SkiaImage::SkiaImage() = default;

SkiaImage::SkiaImage(const scoped_refptr<Bitmap>& bitmap, float scale)
    : image_(gfx::ImageSkia::CreateFromBitmap(bitmap->sk_bitmap(), scale)) {}

SkiaImage::~SkiaImage() = default;

gfx::Size SkiaImage::GetSize() const {
  return image_.size();
}

void SkiaImage::AddBitmapRepresentation(const scoped_refptr<Bitmap>& bitmap,
                                        float scale) {
  image_.AddRepresentation(gfx::ImageSkiaRep(bitmap->sk_bitmap(), scale));
}

void SkiaImage::RemoveRepresentation(float scale) {
  image_.RemoveRepresentation(scale);
}

bool SkiaImage::HasRepresentation(float scale) const {
  return image_.HasRepresentation(scale);
}

}  // namespace hime
