// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/badge.h"

#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/badge.h"

namespace hime {

namespace {

class BadgeImpl : public ViewEventDispatcher<hime::Badge, views::Badge> {
 public:
  explicit BadgeImpl(hime::Badge* delegate) : ViewBaseDispatcher(delegate) {}
};

}  // namespace

Badge::Badge() : View(std::make_unique<BadgeImpl>(this), LayoutType::kLeaf) {
  UsePreferredSizeForYogaMeasurement();
}

Badge::~Badge() = default;

void Badge::SetText(const std::u16string& text) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->SetText(text);
}

const std::u16string& Badge::GetText() const {
  HIME_RETURN_VALUE_ON_DESTROYED_VIEW(this, base::EmptyString16());
  return GetView()->GetText();
}

views::Badge* Badge::GetView() const {
  return static_cast<views::Badge*>(view());
}

}  // namespace hime
