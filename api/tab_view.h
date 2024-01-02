// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_TAB_VIEW_H_
#define CHROHIME_API_TAB_VIEW_H_

#include <vector>

#include "chrohime/api/view.h"

namespace views {
class TabbedPane;
}

namespace hime {

class CHROHIME_EXPORT TabView : public View {
 public:
  TabView();

  void AddPage(const std::u16string& title, scoped_refptr<View> view);
  void AddPageAt(const std::u16string& title, scoped_refptr<View> view,
                 size_t index);
  size_t PageCount() const;
  View* PageAt(size_t index) const;

  void SelectPageAt(size_t index);
  size_t GetSelectedPageIndex() const;
  View* GetSelectedPage() const;

 protected:
  ~TabView() override;

 private:
  views::TabbedPane* GetView() const;

  std::vector<scoped_refptr<View>> pages_;
};

}  // namespace hime

#endif  // CHROHIME_API_TAB_VIEW_H_
