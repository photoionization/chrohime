// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/path.h"

#include "ui/gfx/geometry/skia_conversions.h"

namespace hime {

Path::Path() : path_(std::make_unique<SkPath>()) {}

Path::~Path() = default;

void Path::Reset() {
  path_->reset();
}

bool Path::IsEmpty() const {
  return path_->isEmpty();
}

void Path::Close() {
  path_->close();
}

void Path::MoveTo(const gfx::PointF& point) {
  path_->moveTo(point.x(), point.y());
}

void Path::LineTo(const gfx::PointF& point) {
  path_->lineTo(point.x(), point.y());
}

void Path::QuadTo(const gfx::PointF& point1, const gfx::PointF& point2) {
  path_->quadTo(point1.x(), point1.y(), point2.x(), point2.y());
}

void Path::ConicTo(const gfx::PointF& point1, const gfx::PointF& point2,
                   float weight) {
  path_->conicTo(point1.x(), point1.y(), point2.x(), point2.y(), weight);
}

void Path::CubicTo(const gfx::PointF& point1, const gfx::PointF& point2,
                   const gfx::PointF& point3) {
  path_->cubicTo(point1.x(), point1.y(), point2.x(), point2.y(),
                 point3.x(), point3.y());
}

void Path::ArcTo(const gfx::PointF& point, float rx, float ry, float rotate,
                 ArcSize arc_size, Direction sweep) {
  path_->arcTo(rx, ry, rotate, static_cast<SkPath::ArcSize>(arc_size), sweep,
               point.x(), point.y());
}

void Path::ArcToWithOval(const gfx::RectF& oval, float start_angle,
                         float sweep_angle, bool force_move_to) {
  path_->arcTo(gfx::RectFToSkRect(oval), start_angle, sweep_angle,
               force_move_to);
}

void Path::ArcToWithPoints(const gfx::PointF& point1, const gfx::PointF& point2,
                           float radius) {
  path_->arcTo(point1.x(), point1.y(), point2.x(), point2.y(), radius);
}

void Path::AddRect(const gfx::RectF& rect, Direction direction,
                   unsigned start) {
  path_->addRect(gfx::RectFToSkRect(rect), direction, start);
}

void Path::AddOval(const gfx::RectF& oval, Direction direction,
                   unsigned start) {
  path_->addOval(gfx::RectFToSkRect(oval), direction, start);
}

void Path::AddCircle(const gfx::PointF& center, float radius,
                     Direction direction) {
  path_->addCircle(center.x(), center.y(), radius, direction);
}

void Path::AddArc(const gfx::RectF& oval,
                  float start_angle, float sweep_angle) {
  path_->addArc(gfx::RectFToSkRect(oval), start_angle, sweep_angle);
}

void Path::AddRoundRect(const gfx::RectF& rect, float rx, float ry,
                        Direction direction) {
  path_->addRoundRect(gfx::RectFToSkRect(rect), rx, ry, direction);
}

}  // namespace hime
