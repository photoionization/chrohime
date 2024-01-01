// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/textarea.h"

#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/textarea/textarea.h"

namespace hime {

namespace {

class TextareaImpl : public ViewEventDispatcher<hime::Textarea,
                                                views::Textarea> {
 public:
  explicit TextareaImpl(hime::Textarea* delegate)
      : ViewBaseDispatcher(delegate) {}
};

}  // namespace

Textarea::Textarea() : Textfield(std::make_unique<TextareaImpl>(this)) {}

Textarea::~Textarea() = default;

views::Textarea* Textarea::GetView() const {
  return static_cast<views::Textarea*>(view());
}

}  // namespace hime
