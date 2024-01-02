// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/view.h"

#include "base/ranges/algorithm.h"
#include "chrohime/api/background.h"
#include "chrohime/api/border.h"
#include "chrohime/api/yoga_layout_manager.h"
#include "chrohime/api/yoga_util.h"
#include "chrohime/api/state.h"
#include "chrohime/api/view_event_dispatcher.h"
#include "ui/display/screen.h"
#include "ui/views/background.h"
#include "ui/views/border.h"
#include "ui/views/view.h"
#include "ui/views/widget/widget.h"
#include "yoga/Yoga.h"

namespace hime {

namespace {

class ViewImpl : public ViewEventDispatcher<hime::View, views::View> {
 public:
  explicit ViewImpl(hime::View* delegate) : ViewBaseDispatcher(delegate) {}
};

// Convert case to lower and remove non-ASCII characters.
std::u16string StripStyleName(std::u16string_view name) {
  std::u16string parsed;
  parsed.reserve(name.size());
  for (char c : name) {
    if (base::IsAsciiAlpha(c))
      parsed.push_back(base::ToLowerASCII(c));
  }
  return parsed;
}

// Return the preferred size in yoga measurement.
YGSize MeasureView(YGNodeConstRef node,
                   float width, YGMeasureMode mode,
                   float height, YGMeasureMode height_mode) {
  auto* view = static_cast<View*>(YGNodeGetContext(node));
  gfx::Size size = view->GetPreferredSizeFor({static_cast<int>(width),
                                              static_cast<int>(height)});
  return {static_cast<float>(size.width()), static_cast<float>(size.height())};
}

}  // namespace

// static
View* View::FromViews(views::View* view) {
  return State::GetCurrent()->GetViewFromViewsView(view);
}

View::View() : View(std::make_unique<ViewImpl>(this), LayoutType::kContainer) {}

View::View(std::unique_ptr<views::View> to_take, LayoutType layout_type)
    : layout_type_(layout_type),
      yoga_config_(YGConfigNew()),
      yoga_node_(YGNodeNewWithConfig(yoga_config_)),
      ownership_(std::move(to_take)),
      view_(ownership_.get()) {
  view_->AddObserver(this);
  State::GetCurrent()->views_map_[view_] = this;
  YGNodeSetContext(yoga_node_, this);
  if (layout_type == LayoutType::kContainer) {
    view_->SetLayoutManager(std::make_unique<YogaLayoutManager>(this));
  }
}

View::~View() {
  if (view_) {
    State::GetCurrent()->views_map_.erase(view_);
    // Unsubscribe so OnViewIsDeleting will not be called for self-destruction.
    view_->RemoveObserver(this);
  }
  YGNodeFree(yoga_node_);
  YGConfigFree(yoga_config_);
}

void View::SetVisible(bool visible) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetVisible(visible);
}

bool View::IsVisible() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return view_->GetVisible();
}

bool View::IsDrawn() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return view_->IsDrawn();
}

void View::SetEnabled(bool enabled) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetEnabled(enabled);
}

bool View::IsEnabled() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return view_->GetEnabled();
}

void View::Focus() {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->RequestFocus();
}

bool View::HasFocus() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, false);
  return view_->HasFocus();
}

void View::SetFocusBehavior(FocusBehavior behavior) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  views::View::FocusBehavior b;
  switch (behavior) {
    case FocusBehavior::kNever:
      b = views::View::FocusBehavior::NEVER;
      break;
    case FocusBehavior::kAlways:
      b = views::View::FocusBehavior::ALWAYS;
      break;
    case FocusBehavior::kAccessibleOnly:
      b = views::View::FocusBehavior::ACCESSIBLE_ONLY;
      break;
  }
  view_->SetFocusBehavior(b);
}

View::FocusBehavior View::GetFocusBehavior() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, FocusBehavior::kNever);
  switch (view_->GetFocusBehavior()) {
    case views::View::FocusBehavior::NEVER:
      return FocusBehavior::kNever;
    case views::View::FocusBehavior::ALWAYS:
      return FocusBehavior::kAlways;
    case views::View::FocusBehavior::ACCESSIBLE_ONLY:
      return FocusBehavior::kAccessibleOnly;
  }
}

void View::SetBounds(const gfx::Rect& bounds) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetBoundsRect(bounds);
}

gfx::Rect View::GetBounds() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, gfx::Rect());
  return view_->bounds();
}

gfx::Rect View::GetBoundsInScreen() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, gfx::Rect());
  return view_->GetBoundsInScreen();
}

void View::SetPreferredSize(absl::optional<gfx::Size> size) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetPreferredSize(std::move(size));
}

gfx::Size View::GetPreferredSize() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, gfx::Size());
  return view_->GetPreferredSize();
}

void View::SetStyle(std::u16string_view name, std::u16string_view value) {
  SetYogaProperty(yoga_node_, StripStyleName(name), value);
}

void View::SetStyleNumber(std::u16string_view name, float value) {
  SetYogaProperty(yoga_node_, StripStyleName(name), value);
}

void View::Layout() {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  if (!parent_)
    return;
  view_->InvalidateLayout();
  // When changing the style of a child view, we must re-calculate the layout
  // from the root node.
  View* root = parent_;
  while (!root->IsRootYogaNode() && root->parent_)
    root = root->parent_;
  root->view_->Layout();
}

void View::SchedulePaint() {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SchedulePaint();
}

void View::SchedulePaintInRect(const gfx::Rect& dirty) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SchedulePaintInRect(dirty);
}

void View::SetBackground(scoped_refptr<Background> background) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetBackground(background->TransferOwnership());
}

void View::SetBorder(scoped_refptr<Border> border) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetBorder(border->TransferOwnership());
}

void View::AddChildView(scoped_refptr<View> view) {
  AddChildViewAt(std::move(view), ChildCount());
}

void View::AddChildViewAt(scoped_refptr<View> view, size_t index) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  HIME_RETURN_ON_DESTROYED_VIEW(view);
  view->parent_ = this;
  view_->AddChildViewAt(view->TransferOwnership(), index);
  YGNodeInsertChild(yoga_node(), view->yoga_node(), index);
  children_.insert(children_.begin() + index, std::move(view));
  Layout();
}

void View::RemoveChildView(View* view) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view->parent_ = nullptr;
  view_->RemoveChildView(view->view());
  YGNodeRemoveChild(yoga_node(), view->yoga_node());
  children_.erase(base::ranges::find(children_, view));
  Layout();
}

size_t View::ChildCount() const {
  return children_.size();
}

View* View::ChildAt(size_t index) const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, nullptr);
  if (index < children_.size())
    return children_[index].get();
  else
    return nullptr;
}

void View::SetAccessibleName(const std::u16string& name) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetAccessibleName(name);
}

const std::u16string& View::GetAccessibleName() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return view_->GetAccessibleName();
}

void View::SetId(int id) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetID(id);
}

int View::GetId() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, 0);
  return view_->GetID();
}

View* View::GetViewById(int id) {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, nullptr);
  return FromViews(view_->GetViewByID(id));
}

void View::SetGroup(int group) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  view_->SetGroup(group);
}

int View::GetGroup() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, -1);
  return view_->GetGroup();
}

std::vector<View*> View::GetViewsInGroup(int group) {
  std::vector<View*> result;
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, {});
  views::View::Views views;
  view_->GetViewsInGroup(group, &views);
  for (views::View* view : views)
    result.push_back(FromViews(view));
  return result;
}

gfx::Size View::GetPreferredSizeFor(const gfx::Size& size) const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, gfx::Size());
  return view_->GetPreferredSize(views::SizeBounds(size));
}

bool View::IsRootYogaNode() const {
  return layout_type_ == View::LayoutType::kLeaf ||
         !YGNodeGetParent(yoga_node_);
}

std::unique_ptr<views::View> View::TransferOwnership() {
  DCHECK(ownership_);
  return std::move(ownership_);
}

void View::UsePreferredSizeForYogaMeasurement() {
  DCHECK(!mark_dirty_on_preferred_size_change_);
  mark_dirty_on_preferred_size_change_ = true;
  YGNodeSetMeasureFunc(yoga_node_, MeasureView);
}

void View::OnViewPreferredSizeChanged(views::View* observed_view) {
  if (mark_dirty_on_preferred_size_change_)
    YGNodeMarkDirty(yoga_node_);
}

void View::OnViewIsDeleting(views::View* observed_view) {
  // This method is called when the ownership has been transferred to other
  // classes and then gets deleted there.
  DCHECK(!ownership_);
  State::GetCurrent()->views_map_.erase(view_);
  view_ = nullptr;
}

void View::OnViewVisibilityChanged(views::View* observed_view,
                                   views::View* starting_view) {
  YGNodeStyleSetDisplay(yoga_node_,
                        view_->GetVisible() ? YGDisplayFlex : YGDisplayNone);
}

void View::OnViewAddedToWidget(views::View* observed_view) {
  const display::Screen* const screen = display::Screen::GetScreen();
  DCHECK(screen);
  YGConfigSetPointScaleFactor(
      yoga_config_,
      // TODO(zcbenz): Cache scale factor in window.
      screen->GetDisplayNearestView(view_->GetWidget()->GetNativeView())
          .device_scale_factor());
}

}  // namespace hime
