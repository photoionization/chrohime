// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/table_view.h"

#include "chrohime/api/table_model.h"
#include "ui/views/controls/table/table_view.h"

namespace hime {

TableView::TableView()
    : View(std::make_unique<views::TableView>(), LayoutType::kLeaf),
      table_(static_cast<views::TableView*>(view())) {}

TableView::~TableView() = default;

void TableView::SetColumns(const std::vector<Column>& columns) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  if (model_) {
    NOTREACHED() << "SetColumns must be called before SetModel";
    return;
  }
  table_->SetColumns(columns);
}

void TableView::SetModel(scoped_refptr<TableModel> model) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  table_->SetModel(model->model());
  model_ = std::move(model);
}

TableModel* TableView::GetModel() const {
  return model_.get();
}

}  // namespace hime
