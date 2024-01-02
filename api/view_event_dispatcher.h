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
class ViewBaseDispatcher : public V {
 public:
  // This constructor must be explicitly defined otherwise derived classes would
  // have to explicitly construct all its base classes.
  ViewBaseDispatcher() {
    CHECK(false) <<
        "The default constructor of ViewBaseDispatcher is not supposed to be "
        "actualled called, this usually happens when you does not pass a "
        "delegate when constructing ViewBaseDispatcher in the derived class.";
  }

  template<typename... Arg>
  ViewBaseDispatcher(T* delegate, Arg&&... args)
      : V(std::forward<Arg>(args)...), delegate_(delegate) {}

  T* delegate() const { return delegate_; }

 private:
  raw_ptr<T> delegate_;
};

// Ideally every class should just inherit from ViewEventDispatcher, but some
// views mark OnPaint as final (i.e. Button) and it is impossible to implement
// the on_draw event for them.
template<typename T, typename V>
class ViewOnPaintDispatcher : virtual public ViewBaseDispatcher<T, V> {
 public:
  using ViewBaseDispatcher<T, V>::delegate;

  ViewOnPaintDispatcher() {}

  // views::View:
  void OnPaint(gfx::Canvas* canvas) override {
    Painter painter(canvas);
    if (delegate()->on_will_draw.Emit(delegate(), &painter))
      return;
    V::OnPaint(canvas);
    delegate()->on_draw.Emit(delegate(), &painter);
  }
};

template<typename T, typename V>
class ViewOnMouseDispatcher : virtual public ViewBaseDispatcher<T, V> {
 public:
  using ViewBaseDispatcher<T, V>::delegate;

  ViewOnMouseDispatcher() {}

  // views::View:
  bool OnMousePressed(const ui::MouseEvent& event) override {
    const MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_down.Emit(delegate(), &wrapper))
      return true;
    return V::OnMousePressed(event);
  }

  void OnMouseReleased(const ui::MouseEvent& event) override {
    const MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_up.Emit(delegate(), &wrapper))
      return;
    V::OnMouseReleased(event);
  }

  void OnMouseMoved(const ui::MouseEvent& event) override {
    const MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_move.Emit(delegate(), &wrapper))
      return;
    V::OnMouseMoved(event);
  }

  void OnMouseEntered(const ui::MouseEvent& event) override {
    const MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_enter.Emit(delegate(), &wrapper))
      return;
    V::OnMouseEntered(event);
  }

  void OnMouseExited(const ui::MouseEvent& event) override {
    const MouseEvent wrapper(&const_cast<ui::MouseEvent&>(event));
    if (delegate()->on_mouse_leave.Emit(delegate(), &wrapper))
      return;
    V::OnMouseExited(event);
  }
};

template<typename T, typename V>
class ViewOnKeyDispatcher : virtual public ViewBaseDispatcher<T, V> {
 public:
  using ViewBaseDispatcher<T, V>::delegate;

  ViewOnKeyDispatcher() {}

  // views::View:
  bool OnKeyPressed(const ui::KeyEvent& event) override {
    const KeyEvent wrapper(&const_cast<ui::KeyEvent&>(event));
    if (delegate()->on_key_down.Emit(delegate(), &wrapper))
      return true;
    return V::OnKeyPressed(event);
  }

  bool OnKeyReleased(const ui::KeyEvent& event) override {
    const KeyEvent wrapper(&const_cast<ui::KeyEvent&>(event));
    if (delegate()->on_key_up.Emit(delegate(), &wrapper))
      return true;
    return V::OnKeyReleased(event);
  }
};

// The event dispatcher which supports all input event handlers.
template<typename T, typename V>
class ViewInputEventDispatcher : public ViewOnMouseDispatcher<T, V>,
                                 public ViewOnKeyDispatcher<T, V> {
 public:
  ViewInputEventDispatcher() {}
};

// The default event dispatcher which supports all event handlers.
template<typename T, typename V>
class ViewEventDispatcher : public ViewOnPaintDispatcher<T, V>,
                            public ViewOnMouseDispatcher<T, V>,
                            public ViewOnKeyDispatcher<T, V> {
 public:
  ViewEventDispatcher() {}
};

}  // namespace hime

#endif  // API_VIEW_EVENT_DISPATCHER_H_
