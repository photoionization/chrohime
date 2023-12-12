// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_SCROLL_VIEW_H_
#define CHROHIME_API_SCROLL_VIEW_H_

#include "chrohime/api/view.h"

namespace views {
class ScrollView;
}

namespace hime {

class TableView;

class CHROHIME_EXPORT ScrollView : public View {
 public:
  ScrollView();
  explicit ScrollView(scoped_refptr<TableView> table);

  void SetContentView(scoped_refptr<View> view);
  View* GetContentView() const;

 protected:
  ~ScrollView() override;

  scoped_refptr<View> content_view_;
  raw_ptr<views::ScrollView> scroll_view_;
};

}  // namespace hime

#endif  // CHROHIME_API_SCROLL_VIEW_H_
