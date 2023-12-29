// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHORHIME_API_GEOMETRY_F_H_
#define CHORHIME_API_GEOMETRY_F_H_

namespace gfx {
class PointF;
class RectF;
class Vector2dF;
}

// This may look a weird decision, but rectf_t looks way better than rect_f_t
// in C APIs.
namespace gfx {
using Pointf = PointF;
using Rectf = RectF;
using Vector2df = Vector2dF;
}

#endif  // CHORHIME_API_GEOMETRY_F_H_
