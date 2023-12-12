// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_SIMPLE_TABLE_MODEL_H_
#define CHROHIME_API_SIMPLE_TABLE_MODEL_H_

#include <vector>

#include "chrohime/api/table_model.h"

namespace hime {

class SimpleTableModelImpl;

class CHROHIME_EXPORT SimpleTableModel : public TableModel {
 public:
  using Row = std::vector<std::u16string>;

  SimpleTableModel();

  void AddRow(std::vector<std::u16string> row);
  void RemoveRowAt(size_t index);

 protected:
  ~SimpleTableModel() override;

 private:
  std::unique_ptr<SimpleTableModelImpl> impl_;
};

}  // namespace hime

#endif  // CHROHIME_API_SIMPLE_TABLE_MODEL_H_
