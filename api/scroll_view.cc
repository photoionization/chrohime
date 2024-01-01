// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/scroll_view.h"

#include "chrohime/api/table_view.h"
#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/table/table_view.h"

namespace hime {

namespace {

class ScrollViewImpl : public ViewEventDispatcher<hime::ScrollView,
                                                  views::ScrollView> {
 public:
  explicit ScrollViewImpl(hime::ScrollView* delegate)
      : ViewBaseDispatcher(delegate) {}
};

}  // namespace

ScrollView::ScrollView()
    : ScrollView(std::make_unique<ScrollViewImpl>(this)) {}

ScrollView::ScrollView(scoped_refptr<TableView> table)
    : ScrollView(views::TableView::CreateScrollViewWithTable(
                     std::unique_ptr<views::TableView>(
                        static_cast<views::TableView*>(
                            table->TransferOwnership().release())))) {
  content_view_ = std::move(table);
}

ScrollView::ScrollView(std::unique_ptr<views::ScrollView> to_take)
    : View(std::move(to_take), LayoutType::kLeaf) {}

ScrollView::~ScrollView() = default;

void ScrollView::SetContentView(scoped_refptr<View> view) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  HIME_RETURN_ON_DESTROYED_VIEW(view);
  GetView()->SetContents(view->TransferOwnership());
  content_view_ = std::move(view);
}

View* ScrollView::GetContentView() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, nullptr);
  return content_view_.get();
}

views::ScrollView* ScrollView::GetView() const {
  return static_cast<views::ScrollView*>(view());
}

}  // namespace hime
