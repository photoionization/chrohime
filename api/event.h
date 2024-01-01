// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_EVENT_H_
#define CHROHIME_API_EVENT_H_

#include "base/memory/raw_ptr.h"
#include "chrohime/chrohime_export.h"
#include "ui/events/event.h"

namespace hime {

class CHROHIME_EXPORT Event {
 public:
  enum Flag : int {
    kNone = ui::EF_NONE,
    kIsSynthesized = ui::EF_IS_SYNTHESIZED,
    kShiftDown = ui::EF_SHIFT_DOWN,
    kControlDown = ui::EF_CONTROL_DOWN,
    kAltDown = ui::EF_ALT_DOWN,
    kCommandDown = ui::EF_COMMAND_DOWN,
    kFunctionDown = ui::EF_FUNCTION_DOWN,
    kAltgrDown = ui::EF_ALTGR_DOWN,
    kMod3Down = ui::EF_MOD3_DOWN,
    kNumLockOn = ui::EF_NUM_LOCK_ON,
    kCapsLockOn = ui::EF_CAPS_LOCK_ON,
    kScrollLockOn = ui::EF_SCROLL_LOCK_ON,
    kLeftMouseButton = ui::EF_LEFT_MOUSE_BUTTON,
    kMiddleMouseButton = ui::EF_MIDDLE_MOUSE_BUTTON,
    kRightMouseButton = ui::EF_RIGHT_MOUSE_BUTTON,
    kBackMouseButton = ui::EF_BACK_MOUSE_BUTTON,
    kForwardMouseButton = ui::EF_FORWARD_MOUSE_BUTTON,
  };

  enum class Type {
    kUnknown = ui::ET_UNKNOWN,
    kMouseDown = ui::ET_MOUSE_PRESSED,
    kMouseDrag = ui::ET_MOUSE_DRAGGED,
    kMouseUp = ui::ET_MOUSE_RELEASED,
    kMouseMove = ui::ET_MOUSE_MOVED,
    kMouseEnter = ui::ET_MOUSE_ENTERED,
    kMouseLeave = ui::ET_MOUSE_EXITED,
    kKeyDown = ui::ET_KEY_PRESSED,
    kKeyUp = ui::ET_KEY_RELEASED,
  };

  explicit Event(ui::Event* event) : event_(event) {}

  Type GetType() const { return static_cast<Type>(event()->type()); }
  int GetFlags() const { return event()->flags(); }

  bool IsCancelable() const { return event()->cancelable(); }
  void SetHandled() { event()->SetHandled(); }
  bool IsHandled() const { return event()->handled(); }
  void StopPropagation() { event()->StopPropagation(); }
  bool IsConsumed() const { return event()->stopped_propagation(); }
  void SetSkipped() { event()->SetSkipped(); }

  ui::Event* event() const { return event_; }

 private:
  const raw_ptr<ui::Event> event_;
};

class CHROHIME_EXPORT LocatedEvent : public Event {
 public:
  explicit LocatedEvent(ui::LocatedEvent* event) : Event(event) {}

  gfx::PointF GetLocation() const { return event()->location_f(); }
  gfx::PointF GetRootLocation() const { return event()->root_location_f(); }

  ui::LocatedEvent* event() const {
    return static_cast<ui::LocatedEvent*>(Event::event());
  }
};

class CHROHIME_EXPORT MouseEvent : public LocatedEvent {
 public:
  explicit MouseEvent(ui::MouseEvent* event) : LocatedEvent(event) {}

  int GetClickCount() const { return event()->GetClickCount(); }
  const gfx::Vector2dF& GetMovement() const { return event()->movement(); }

  ui::MouseEvent* event() const {
    return static_cast<ui::MouseEvent*>(LocatedEvent::event());
  }
};

}  // namespace hime

#endif  // CHROHIME_API_EVENT_H_
