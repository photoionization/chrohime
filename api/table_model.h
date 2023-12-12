// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_TABLE_MODEL_H_
#define CHROHIME_API_TABLE_MODEL_H_

#include "chrohime/api/object.h"

namespace ui {
class TableModel;
}

namespace hime {

class CHROHIME_EXPORT TableModel : public Object {
 public:
  ui::TableModel* model() const { return model_.get(); }

 protected:
  // Due the restrictions of ui::TableModel class, this class does not manage
  // the lifetime of model_, instead each subclass is responsible for releasing
  // the model_'s memory.
  explicit TableModel(ui::TableModel* weak_ref);
  ~TableModel() override;

 private:
  raw_ptr<ui::TableModel> model_;
};

}  // namespace hime

#endif  // CHROHIME_API_TABLE_MODEL_H_
