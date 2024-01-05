// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/painter.h"

#include "chrohime/api/path.h"
#include "chrohime/api/skia_image.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/gfx/image/image_skia_rep.h"
#include "ui/gfx/skia_paint_util.h"

namespace hime {

namespace {

cc::PaintFlags PaintToFlags(const Paint& paint) {
  cc::PaintFlags flags;
  flags.setStyle(static_cast<cc::PaintFlags::Style>(paint.style));
  flags.setColor(paint.color);
  flags.setStrokeWidth(paint.stroke_width);
  flags.setStrokeCap(static_cast<cc::PaintFlags::Cap>(paint.stroke_cap));
  flags.setBlendMode(paint.blend_mode);
  flags.setAntiAlias(paint.anti_alias);
  return flags;
}

}  // namespace

Painter::Painter(gfx::Canvas* canvas) : canvas_(canvas) {}

Painter::~Painter() = default;

void Painter::Save() {
  canvas_->Save();
}

void Painter::Restore() {
  canvas_->Restore();
}

void Painter::ClipRect(const gfx::RectF& rect, SkClipOp op) {
  canvas_->ClipRect(rect, op);
}

void Painter::ClipPath(const scoped_refptr<Path>& path, SkClipOp op,
                       bool anti_alias) {
  canvas_->sk_canvas()->clipPath(path->sk_path(), op, anti_alias);
}

void Painter::Translate(const gfx::Vector2dF& offset) {
  canvas_->sk_canvas()->translate(offset.x(), offset.y());
}

void Painter::Scale(float x_scale, float y_scale) {
  canvas_->Scale(x_scale, y_scale);
}

void Painter::Rotate(float angle) {
  canvas_->sk_canvas()->rotate(angle);
}

void Painter::DrawColor(SkColor color, SkBlendMode mode) {
  canvas_->DrawColor(color, mode);
}

void Painter::DrawLine(const gfx::PointF& p1, const gfx::PointF& p2,
                       const Paint& paint) {
  canvas_->DrawLine(p1, p2, PaintToFlags(paint));
}

void Painter::DrawRect(const gfx::RectF& rect, const Paint& paint) {
  canvas_->DrawRect(rect, PaintToFlags(paint));
}

void Painter::DrawOval(const gfx::RectF& rect, const Paint& paint) {
  canvas_->sk_canvas()->drawOval(gfx::RectFToSkRect(rect), PaintToFlags(paint));
}

void Painter::DrawRoundRect(const gfx::RectF& rect, float radius,
                            const Paint& paint) {
  canvas_->DrawRoundRect(rect, radius, PaintToFlags(paint));
}

void Painter::DrawPath(const scoped_refptr<Path>& path, const Paint& paint) {
  canvas_->DrawPath(path->sk_path(), PaintToFlags(paint));
}

void Painter::DrawImageAt(const scoped_refptr<SkiaImage>& image,
                          const gfx::PointF& point,
                          const Paint& paint) {
  const gfx::ImageSkiaRep& image_rep = image->image_skia().GetRepresentation(
      canvas_->image_scale());
  if (image_rep.is_null())
    return;

  auto* sk_canvas = canvas_->sk_canvas();
  float bitmap_scale = image_rep.scale();
  Save();
  sk_canvas->scale(1.f / bitmap_scale, 1.f / bitmap_scale);
  sk_canvas->translate(point.x() * bitmap_scale, point.y() * bitmap_scale);
  sk_canvas->saveLayer(PaintToFlags(paint));
  sk_canvas->drawPicture(image_rep.GetPaintRecord());
  sk_canvas->restore();
  Restore();
}

void Painter::DrawImage(const scoped_refptr<SkiaImage>& image,
                        const gfx::RectF& src,
                        const gfx::RectF& dest,
                        const Paint& paint) {
  if (src.IsEmpty())
    return;
  SkRect clip;
  SkRect dest_rect = gfx::RectFToSkRect(dest);
  if (canvas_->sk_canvas()->getLocalClipBounds(&clip) &&
      clip.intersects(dest_rect)) {
    return;
  }

  const gfx::ImageSkiaRep& image_rep = image->image_skia().GetRepresentation(
      canvas_->image_scale());
  if (image_rep.is_null())
    return;

  // Make a bitmap shader that contains the bitmap we want to draw. This is
  // basically what SkCanvas.drawBitmap does internally, but it gives us
  // more control over quality and will use the mipmap in the source image if
  // it has one, whereas drawBitmap won't.
  SkMatrix shader_scale;
  shader_scale.setScale(dest.width() / src.width(),
                        dest.height() / src.height());
  shader_scale.preTranslate(-src.x(), -src.y());
  shader_scale.postTranslate(dest.x(), dest.y());

  cc::PaintFlags flags(PaintToFlags(paint));
  flags.setShader(gfx::CreateImageRepShaderForScale(
      image_rep, SkTileMode::kRepeat, SkTileMode::kRepeat, shader_scale,
      image_rep.scale()));

  // The rect will be filled by the bitmap.
  canvas_->sk_canvas()->drawRect(dest_rect, flags);
}

}  // namespace hime
