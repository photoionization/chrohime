// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/table_model.h"

#include "ui/base/models/table_model.h"

namespace hime {

TableModel::TableModel(ui::TableModel* weak_ref) : model_(weak_ref) {}

TableModel::~TableModel() = default;

}  // namespace hime
