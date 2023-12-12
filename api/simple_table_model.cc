// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/simple_table_model.h"

#include "ui/base/models/table_model.h"

namespace hime {

class SimpleTableModelImpl : public ui::TableModel {
 public:
  using Row = SimpleTableModel::Row;

  SimpleTableModelImpl() = default;

  void AddRow(std::vector<std::u16string> row) {
    rows_.push_back(std::move(row));
  }

  void RemoveRowAt(size_t index) {
    if (index < RowCount())
      rows_.erase(rows_.begin() + index);
  }

  // ui::TableModel:
  size_t RowCount() override {
    return rows_.size();
  }

  std::u16string GetText(size_t row_index, int column_index) override {
    if (row_index < RowCount()) {
      const Row& row = rows_[row_index];
      if (column_index >= 0 && static_cast<size_t>(column_index) < row.size())
        return row[column_index];
    }
    return u"(no data)";
  }

  void SetObserver(ui::TableModelObserver* observer) override {
    observer_ = observer;
  }

 private:
  std::vector<Row> rows_;
  raw_ptr<ui::TableModelObserver> observer_{nullptr};
};

SimpleTableModel::SimpleTableModel()
    : TableModel(new SimpleTableModelImpl),
      impl_(static_cast<SimpleTableModelImpl*>(model())) {}

SimpleTableModel::~SimpleTableModel() = default;

void SimpleTableModel::AddRow(std::vector<std::u16string> row) {
  impl_->AddRow(std::move(row));
}

void SimpleTableModel::RemoveRowAt(size_t index) {
  impl_->RemoveRowAt(index);
}

}  // namespace hime
