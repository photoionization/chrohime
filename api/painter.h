// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_PAINTER_H_
#define CHROHIME_API_PAINTER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "chrohime/chrohime_export.h"
#include "third_party/skia/include/core/SkBlendMode.h"
#include "third_party/skia/include/core/SkClipOp.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkPaint.h"

namespace gfx {
class Canvas;
class PointF;
class RectF;
class Vector2dF;
}

namespace hime {

class Path;
class SkiaImage;

// This struct maps to cc::PaintFlags, but we use Paint as name to match the
// naming style of skia.
struct Paint {
  enum class Style : uint8_t {
    kFill = SkPaint::kFill_Style,
    kStroke = SkPaint::kStroke_Style,
    kStrokeAndFill = SkPaint::kStrokeAndFill_Style,
  };

  enum class Cap {
    kButt = SkPaint::kButt_Cap,
    kRound = SkPaint::kRound_Cap,
    kSquare = SkPaint::kSquare_Cap,
  };

  Style style = Style::kFill;
  SkColor color = SK_ColorBLACK;
  float stroke_width = 1.f;
  Cap stroke_cap = Cap::kButt;
  SkBlendMode blend_mode = SkBlendMode::kSrcOver;
  bool anti_alias = true;
};

class CHROHIME_EXPORT Painter {
 public:
  explicit Painter(gfx::Canvas* canvas);
  ~Painter();

  void Save();
  void Restore();

  void ClipRect(const gfx::RectF& rect, SkClipOp op);
  void ClipPath(const scoped_refptr<Path>& path, SkClipOp op, bool anti_alias);

  void Translate(const gfx::Vector2dF& offset);
  void Scale(float x_scale, float y_scale);
  void Rotate(float angle);

  void DrawColor(SkColor color, SkBlendMode mode);
  void DrawLine(const gfx::PointF& p1, const gfx::PointF& p2,
                const Paint& paint);
  void DrawRect(const gfx::RectF& rect, const Paint& paint);
  void DrawOval(const gfx::RectF& rect, const Paint& paint);
  void DrawRoundRect(const gfx::RectF& rect, float radius, const Paint& paint);
  void DrawPath(const scoped_refptr<Path>& path, const Paint& paint);
  void DrawImageAt(const scoped_refptr<SkiaImage>& image,
                   const gfx::PointF& point,
                   const Paint& paint);
  void DrawImage(const scoped_refptr<SkiaImage>& image,
                 const gfx::RectF& src,
                 const gfx::RectF& dest,
                 const Paint& paint);

 private:
  raw_ptr<gfx::Canvas> canvas_;
};

}  // namespace hime

#endif  // CHROHIME_API_PAINTER_H_
