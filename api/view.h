// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_VIEW_H_
#define CHROHIME_API_VIEW_H_

#include <vector>

#include "base/notreached.h"
#include "chrohime/api/object.h"
#include "chrohime/api/signal.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "ui/views/view_observer.h"

typedef struct YGNode* YGNodeRef;
typedef struct YGConfig* YGConfigRef;

namespace gfx {
class Rect;
class Size;
}

namespace hime {

class Background;
class Border;
class KeyEvent;
class MouseEvent;
class Painter;

class CHROHIME_EXPORT View : public Object,
                             public views::ViewObserver {
 public:
  enum class FocusBehavior {
    kNever,
    kAlways,
    kAccessibleOnly,
  };

  static View* FromViews(views::View* view);

  View();

  void SetVisible(bool visible);
  bool IsVisible() const;
  bool IsDrawn() const;
  void SetEnabled(bool enabled);
  bool IsEnabled() const;
  void Focus();
  bool HasFocus() const;
  void SetFocusBehavior(FocusBehavior behavior);
  FocusBehavior GetFocusBehavior() const;

  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds() const;
  gfx::Rect GetBoundsInScreen() const;
  void SetPreferredSize(absl::optional<gfx::Size> size);
  gfx::Size GetPreferredSize() const;
  void SetStyle(std::u16string_view name, std::u16string_view value);
  void SetStyleNumber(std::u16string_view name, float value);
  void Layout();

  void SchedulePaint();
  void SchedulePaintInRect(const gfx::Rect& dirty);
  void SetBackground(scoped_refptr<Background> background);
  void SetBorder(scoped_refptr<Border> border);

  void AddChildView(scoped_refptr<View> view);
  void AddChildViewAt(scoped_refptr<View> view, size_t index);
  void RemoveChildView(View* view);
  size_t ChildCount() const;
  View* ChildAt(size_t index) const;

  void SetAccessibleName(const std::u16string& name);
  const std::u16string& GetAccessibleName() const;

  void SetId(int id);
  int GetId() const;
  View* GetViewById(int id);
  void SetGroup(int group);
  int GetGroup() const;
  std::vector<View*> GetViewsInGroup(int group);

  // TODO(zcbenz): Expose the method to C after finding a good C API to
  // represent the SizeBounds.
  gfx::Size GetPreferredSizeFor(const gfx::Size& size) const;

  // Internal: Return whether this is the root node when calculating layout.
  bool IsRootYogaNode() const;

  // Internal: Give the ownership of underlying views::View to caller.
  std::unique_ptr<views::View> TransferOwnership();

  // Internal: Yoga layout related properties.
  enum class LayoutType { kContainer, kLeaf };
  LayoutType layout_type() const { return layout_type_; }
  YGNodeRef yoga_node() const { return yoga_node_; }

  // Internal: Return whether the underlying view has been destroyed.
  bool is_destroyed() const { return !view(); }

  // Events.
  Signal<bool(View*, Painter*)> on_will_draw;
  Signal<void(View*, Painter*)> on_draw;
  Signal<bool(View*, const MouseEvent*)> on_mouse_down;
  Signal<bool(View*, const MouseEvent*)> on_mouse_up;
  Signal<bool(View*, const MouseEvent*)> on_mouse_move;
  Signal<bool(View*, const MouseEvent*)> on_mouse_enter;
  Signal<bool(View*, const MouseEvent*)> on_mouse_leave;
  Signal<bool(View*, const KeyEvent*)> on_key_down;
  Signal<bool(View*, const KeyEvent*)> on_key_up;

 protected:
  View(std::unique_ptr<views::View> to_take, LayoutType layout_type);
  ~View() override;

  // Called by sub-classes which have variable preferred sizes.
  void UsePreferredSizeForYogaMeasurement();

  // views::ViewObserver:
  void OnViewPreferredSizeChanged(views::View* observed_view) override;
  void OnViewIsDeleting(views::View* observed_view) override;
  void OnViewVisibilityChanged(views::View* observed_view,
                               views::View* starting_view) override;
  void OnViewAddedToWidget(views::View* observed_view) override;

  // Internal: Get the underlying view.
  // Note that the returned pointer might be null, and caller must check before
  // using to avoid crashes.
  views::View* view() const { return view_.get(); }

 private:
  LayoutType layout_type_;
  YGConfigRef yoga_config_;
  YGNodeRef yoga_node_;
  bool mark_dirty_on_preferred_size_change_ = false;

  // Relationships with other views used for layout.
  raw_ptr<View> parent_ = nullptr;
  std::vector<scoped_refptr<View>> children_;

  // Due to restrictions of ui/views APIs, we have to transfer the ownership of
  // underlying view to other classes under some cases, so we do not manage the
  // ownership all the time.
  // The |view_| points to the views::View wrapped by this class, and may become
  // null if the ownership has been transferred to other classes and then gets
  // destructed. The |ownership_| pointer manages the views::View, and may
  // become null after TransferOwnership() gets called.
  std::unique_ptr<views::View> ownership_;
  raw_ptr<views::View> view_ = nullptr;
};

// Helper for guarding against destroyed views.
#define HIME_RETURN_ON_DESTROYED_VIEW(obj) \
  if (!obj || obj->is_destroyed()) { \
    NOTREACHED() << "View has been destroyed."; \
    return; \
  }
#define HIME_RETURN_VALUE_ON_DESTROYED_VIEW(obj, value) \
  if (!obj || obj->is_destroyed()) { \
    NOTREACHED() << "View has been destroyed."; \
    return value; \
  }

}  // namespace hime

#endif  // CHROHIME_API_VIEW_H_
