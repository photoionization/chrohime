// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/skia_image.h"

#include "base/files/file_util.h"
#include "base/threading/thread_restrictions.h"
#include "chrohime/api/bitmap.h"
#include "chrohime/api/buffer.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/image/image_skia_rep.h"

namespace hime {

// static
scoped_refptr<SkiaImage> SkiaImage::CreateFromFilePath(
    const base::FilePath& path) {
  // TODO(zcbenz): Recognize scale factors in filename.
  base::ScopedAllowBlockingForTesting allow_blocking;
  std::string data;
  if (!base::ReadFileToString(path, &data))
    return nullptr;
  Buffer buffer = Buffer::Wrap(data.data(), data.size());
  scoped_refptr<Bitmap> jpeg = Bitmap::CreateFromJpeg(buffer);
  if (jpeg)
    return base::MakeRefCounted<SkiaImage>(jpeg, 1.f);
  scoped_refptr<Bitmap> png = Bitmap::CreateFromPng(buffer);
  if (png)
    return base::MakeRefCounted<SkiaImage>(png, 1.f);
  return nullptr;
}

SkiaImage::SkiaImage() = default;

SkiaImage::SkiaImage(const scoped_refptr<Bitmap>& bitmap, float scale)
    : image_(gfx::ImageSkia::CreateFromBitmap(bitmap->sk_bitmap(), scale)) {}

SkiaImage::~SkiaImage() = default;

bool SkiaImage::IsEmpty() const {
  return image_.isNull();
}

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
