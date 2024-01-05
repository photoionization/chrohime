// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/bitmap.h"

#include "chrohime/api/buffer.h"
#include "ui/gfx/codec/jpeg_codec.h"
#include "ui/gfx/codec/png_codec.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/size.h"

namespace hime {

// static
scoped_refptr<Bitmap> Bitmap::CreateFromJpeg(const Buffer& buffer) {
  std::unique_ptr<SkBitmap> bitmap = gfx::JPEGCodec::Decode(
      static_cast<unsigned char*>(buffer.content()), buffer.size());
  if (!bitmap)
    return nullptr;
  return base::MakeRefCounted<Bitmap>(std::move(*bitmap));
}

// static
scoped_refptr<Bitmap> Bitmap::CreateFromPng(const Buffer& buffer) {
  SkBitmap bitmap;
  if (!gfx::PNGCodec::Decode(static_cast<unsigned char*>(buffer.content()),
                             buffer.size(),
                             &bitmap)) {
    return nullptr;
  }
  return base::MakeRefCounted<Bitmap>(std::move(bitmap));
}

Bitmap::Bitmap() = default;

Bitmap::Bitmap(SkBitmap bitmap) : bitmap_(std::move(bitmap)) {}

Bitmap::~Bitmap() = default;

gfx::Size Bitmap::GetSize() const {
  return gfx::Size(bitmap_.width(), bitmap_.height());
}

SkColor Bitmap::GetColor(const gfx::Point& point) const {
  return bitmap_.getColor(point.x(), point.y());
}

}  // namespace hime
