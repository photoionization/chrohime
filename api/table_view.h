// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_TABLE_VIEW_H_
#define CHROHIME_API_TABLE_VIEW_H_

#include "chrohime/api/view.h"
#include "ui/base/models/table_model.h"

namespace views {
class TableView;
}

namespace hime {

class TableModel;

class CHROHIME_EXPORT TableView : public View {
 public:
  using Column = ui::TableColumn;

  TableView();

  void SetColumns(const std::vector<Column>& columns);
  void SetModel(scoped_refptr<TableModel> model);
  TableModel* GetModel() const;

 protected:
  ~TableView() override;

 private:
  views::TableView* GetView() const;

  scoped_refptr<TableModel> model_;
};

}  // namespace hime

#endif  // CHROHIME_API_TABLE_VIEW_H_
