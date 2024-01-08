// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/image.h"

#include "chrohime/api/skia_image.h"

namespace hime {

// static
scoped_refptr<Image> Image::CreateFromFilePath(const base::FilePath& path) {
  auto skia_image = SkiaImage::CreateFromFilePath(path);
  if (!skia_image)
    return nullptr;
  return base::MakeRefCounted<Image>(skia_image);
}

Image::Image() = default;

Image::Image(const scoped_refptr<SkiaImage>& skia_image)
    : model_(ui::ImageModel::FromImageSkia(skia_image->image_skia())) {}

Image::~Image() = default;

bool Image::IsEmpty() const {
  return model_.IsEmpty();
}

gfx::Size Image::GetSize() const {
  return model_.Size();
}

}  // namespace hime
