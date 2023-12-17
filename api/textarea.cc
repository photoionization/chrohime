// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/textarea.h"

#include "ui/views/controls/textarea/textarea.h"

namespace hime {

Textarea::Textarea() : Textfield(std::make_unique<views::Textarea>()) {}

Textarea::~Textarea() = default;

views::Textarea* Textarea::GetView() const {
  return static_cast<views::Textarea*>(view());
}

}  // namespace hime
