// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/painter.h"

#include "chrohime/api/path.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/geometry/skia_conversions.h"

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

void Painter::ClipRect(const gfx::Rectf& rect, SkClipOp op) {
  canvas_->ClipRect(rect, op);
}

void Painter::ClipPath(const scoped_refptr<Path>& path, SkClipOp op,
                       bool anti_alias) {
  canvas_->sk_canvas()->clipPath(path->sk_path(), op, anti_alias);
}

void Painter::Translate(const gfx::Vector2df& offset) {
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

void Painter::DrawRect(const gfx::Rectf& rect, const Paint& paint) {
  canvas_->DrawRect(rect, PaintToFlags(paint));
}

void Painter::DrawOval(const gfx::Rectf& rect, const Paint& paint) {
  canvas_->sk_canvas()->drawOval(gfx::RectFToSkRect(rect), PaintToFlags(paint));
}

void Painter::DrawRoundRect(const gfx::Rectf& rect, float radius,
                            const Paint& paint) {
  canvas_->DrawRoundRect(rect, radius, PaintToFlags(paint));
}

void Painter::DrawPath(const scoped_refptr<Path>& path, const Paint& paint) {
  canvas_->DrawPath(path->sk_path(), PaintToFlags(paint));
}

}  // namespace hime
