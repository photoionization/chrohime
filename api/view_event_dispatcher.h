// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_VIEW_EVENT_DISPATCHER_H_
#define CHROHIME_API_VIEW_EVENT_DISPATCHER_H_

#include "chrohime/api/event.h"
#include "chrohime/api/painter.h"

namespace gfx {
class Canvas;
}

namespace hime {

// Convert virtual methods into signals.
template<typename T, typename V>
class ViewEventDispatcher : public V {
 public:
  template<typename... Arg>
  ViewEventDispatcher(T* delegate, Arg&&... args)
      : V(std::forward<Arg>(args)...), delegate_(delegate) {}

  // views::View:
  bool OnMousePressed(const ui::MouseEvent& event) override {
    MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_down.Emit(delegate(), &wrapper))
      return true;
    return V::OnMousePressed(event);
  }

  void OnMouseReleased(const ui::MouseEvent& event) override {
    MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_up.Emit(delegate(), &wrapper))
      return;
    V::OnMouseReleased(event);
  }

  void OnMouseMoved(const ui::MouseEvent& event) override {
    MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_move.Emit(delegate(), &wrapper))
      return;
    V::OnMouseMoved(event);
  }

  void OnMouseEntered(const ui::MouseEvent& event) override {
    MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_enter.Emit(delegate(), &wrapper))
      return;
    V::OnMouseEntered(event);
  }

  void OnMouseExited(const ui::MouseEvent& event) override {
    MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_leave.Emit(delegate(), &wrapper))
      return;
    V::OnMouseExited(event);
  }

  T* delegate() const { return delegate_; }

 private:
  raw_ptr<T> delegate_;
};

// Ideally every class should just inherit from ViewEventDispatcher, but some
// views mark OnPaint as final (i.e. Button) and it is impossible to implement
// the on_draw event for them.
template<typename T, typename V>
class ViewOnPaintDispatcher : public ViewEventDispatcher<T, V> {
 public:
  using ViewEventDispatcher<T, V>::delegate;

  template<typename... Arg>
  ViewOnPaintDispatcher(T* delegate, Arg&&... args)
      : ViewEventDispatcher<T, V>(delegate, std::forward<Arg>(args)...) {}

  // views::View:
  void OnPaint(gfx::Canvas* canvas) override {
    Painter painter(canvas);
    if (delegate()->on_will_draw.Emit(delegate(), &painter))
      return;
    V::OnPaint(canvas);
    delegate()->on_draw.Emit(delegate(), &painter);
  }
};

}  // namespace hime

#endif  // API_VIEW_EVENT_DISPATCHER_H_
