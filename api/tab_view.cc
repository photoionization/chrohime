// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/tab_view.h"

#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/tabbed_pane/tabbed_pane.h"

namespace hime {

namespace {

class TabViewImpl : public ViewOnPaintDispatcher<hime::TabView,
                                                 views::TabbedPane> {
 public:
  explicit TabViewImpl(hime::TabView* delegate)
      : ViewOnPaintDispatcher(delegate) {}
};

}  // namespace

TabView::TabView()
    : View(std::make_unique<TabViewImpl>(this), LayoutType::kLeaf) {}

TabView::~TabView() = default;

void TabView::AddPage(const std::u16string& title, scoped_refptr<View> view) {
  AddPageAt(title, std::move(view), PageCount());
}

void TabView::AddPageAt(const std::u16string& title, scoped_refptr<View> view,
                        size_t index) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->AddTabAtIndex(index, title, view->TransferOwnership());
  pages_.push_back(std::move(view));
}

size_t TabView::PageCount() const {
  return pages_.size();
}

View* TabView::PageAt(size_t index) const {
  if (index < pages_.size())
    return pages_[index].get();
  else
    return nullptr;
}

void TabView::SelectPageAt(size_t index) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SelectTabAt(index);
}

size_t TabView::GetSelectedPageIndex() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, 0);
  return GetView()->GetSelectedTabIndex();
}

View* TabView::GetSelectedPage() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, nullptr);
  return PageAt(GetSelectedPageIndex());
}

views::TabbedPane* TabView::GetView() const {
  return static_cast<views::TabbedPane*>(view());
}

}  // namespace hime
