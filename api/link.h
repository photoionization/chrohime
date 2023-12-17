// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LINK_H_
#define CHROHIME_API_LINK_H_

#include "chrohime/api/label.h"

namespace views {
class Link;
}

namespace hime {

class CHROHIME_EXPORT Link : public Label {
 public:
  Link();

  views::Link* GetView() const;

  // Events.
  Signal<void(Link*)> on_click;

 protected:
  ~Link() override;

  // views::ViewObserver:
  void OnViewPreferredSizeChanged(views::View* observed_view) override;

 private:
  void OnClick();
};

}  // namespace hime

#endif  // CHROHIME_API_LINK_H_
