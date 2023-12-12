// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_STATE_H_
#define CHROHIME_API_STATE_H_

#include <unordered_map>

#include "chrohime/chrohime_export.h"

namespace views {
class View;
}

namespace hime {

class View;

class CHROHIME_EXPORT State {
 public:
  static State* GetCurrent();

  State();
  ~State();

  View* GetViewFromViewsView(views::View* view) const;

 private:
  friend class View;

  // The views::View class does not provide a away to store pointer inside it,
  // so we have to store the relationship in a separate map.
  std::unordered_map<views::View*, View*> views_map_;
};

}  // namespace hime

#endif  // CHROHIME_API_STATE_H_
