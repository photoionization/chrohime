// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/table_view.h"

#include "chrohime/api/table_model.h"
#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/table/table_view.h"

namespace hime {

namespace {

class TableViewImpl : public ViewOnPaintDispatcher<hime::TableView,
                                                   views::TableView> {
 public:
  explicit TableViewImpl(hime::TableView* delegate)
      : ViewOnPaintDispatcher(delegate) {}
};

}  // namespace

TableView::TableView()
    : View(std::make_unique<TableViewImpl>(this), LayoutType::kLeaf) {}

TableView::~TableView() = default;

void TableView::SetColumns(const std::vector<Column>& columns) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  if (model_) {
    NOTREACHED() << "SetColumns must be called before SetModel";
    return;
  }
  GetView()->SetColumns(columns);
}

void TableView::SetModel(scoped_refptr<TableModel> model) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetModel(model->model());
  model_ = std::move(model);
}

TableModel* TableView::GetModel() const {
  return model_.get();
}

views::TableView* TableView::GetView() const {
  return static_cast<views::TableView*>(view());
}

}  // namespace hime
