// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_VIEW_EVENT_DISPATCHER_H_
#define CHROHIME_API_VIEW_EVENT_DISPATCHER_H_

#include "chrohime/api/painter.h"

namespace gfx {
class Canvas;
}

namespace hime {

template<typename T, typename V>
class ViewEventDispatcher : public V {
 public:
  explicit ViewEventDispatcher(T* delegate) : delegate_(delegate) {}

  void OnPaint(gfx::Canvas* canvas) override {
    Painter painter(canvas);
    if (delegate_->on_will_draw.Emit(delegate_, &painter))
      return;
    V::OnPaint(canvas);
    delegate_->on_draw.Emit(delegate_, &painter);
  }

 private:
  raw_ptr<T> delegate_;
};

}  // namespace hime

#endif  // API_VIEW_EVENT_DISPATCHER_H_
