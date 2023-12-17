// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/link.h"

#include "ui/views/controls/link.h"

namespace hime {

Link::Link() : Label(std::make_unique<views::Link>()) {
  GetView()->SetCallback(
      base::BindRepeating(&Link::OnClick, base::Unretained(this)));
}

Link::~Link() = default;

views::Link* Link::GetView() const {
  return static_cast<views::Link*>(view());
}

void Link::OnViewPreferredSizeChanged(views::View* observed_view) {
  Label::OnViewPreferredSizeChanged(observed_view);
  // Link has a fixed maximum width.
  // Without setting max-width the link will take full width in space and the
  // empty spaces will also be clickable.
  SetNumberStyle(u"max-width", GetPreferredSize().width());
}

void Link::OnClick() {
  on_click.Emit(this);
}

}  // namespace hime
