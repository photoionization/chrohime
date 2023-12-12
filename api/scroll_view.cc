// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/scroll_view.h"

#include "chrohime/api/table_view.h"
#include "ui/views/controls/scroll_view.h"
#include "ui/views/controls/table/table_view.h"

namespace hime {

ScrollView::ScrollView()
    : View(std::make_unique<views::ScrollView>(), LayoutType::kLeaf),
      scroll_view_(static_cast<views::ScrollView*>(view())) {}

ScrollView::ScrollView(scoped_refptr<TableView> table)
    : View(views::TableView::CreateScrollViewWithTable(
               std::unique_ptr<views::TableView>(
                  static_cast<views::TableView*>(
                      table->TransferOwnership().release()))),
           LayoutType::kLeaf),
      scroll_view_(static_cast<views::ScrollView*>(view())) {
  content_view_ = std::move(table);
}

ScrollView::~ScrollView() = default;

void ScrollView::SetContentView(scoped_refptr<View> view) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  HIME_RETURN_ON_DESTROYED_VIEW(view);
  scroll_view_->SetContents(view->TransferOwnership());
  content_view_ = std::move(view);
}

View* ScrollView::GetContentView() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, nullptr);
  return content_view_.get();
}

}  // namespace hime
