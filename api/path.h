// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_PATH_H_
#define CHROHIME_API_PATH_H_

#include "chrohime/api/object.h"
#include "third_party/skia/include/core/SkPath.h"

namespace gfx {
class PointF;
class RectF;
}

namespace hime {

class CHROHIME_EXPORT Path : public Object {
 public:
  using Direction = SkPathDirection;

  enum class ArcSize {
    kSmall = SkPath::kSmall_ArcSize,
    kLarge = SkPath::kLarge_ArcSize,
  };

  Path();

  void Reset();
  bool IsEmpty() const;
  void Close();
  void MoveTo(const gfx::PointF& point);
  void LineTo(const gfx::PointF& point);
  void QuadTo(const gfx::PointF& point1, const gfx::PointF& point2);
  void ConicTo(const gfx::PointF& point1, const gfx::PointF& point2,
               float weight);
  void CubicTo(const gfx::PointF& point1, const gfx::PointF& point2,
               const gfx::PointF& point3);
  void ArcTo(const gfx::PointF& point, float rx, float ry, float rotate,
             ArcSize arc_size, Direction direction);
  void ArcToWithOval(const gfx::RectF& oval, float start_angle,
                     float sweep_angle, bool force_move_to);
  void ArcToWithPoints(const gfx::PointF& point1, const gfx::PointF& point2,
                       float radius);
  void AddRect(const gfx::RectF& rect, Direction direction, unsigned start);
  void AddOval(const gfx::RectF& oval, Direction direction, unsigned start);
  void AddCircle(const gfx::PointF& center, float radius, Direction direction);
  void AddArc(const gfx::RectF& oval, float start_angle, float sweep_angle);
  void AddRoundRect(const gfx::RectF& rect, float rx, float ry,
                    Direction direction);

  const SkPath& sk_path() const { return *path_; }

 protected:
  ~Path() override;

 private:
  std::unique_ptr<SkPath> path_;
};

}  // namespace hime

#endif  // CHROHIME_API_PATH_H_
