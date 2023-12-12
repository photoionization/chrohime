// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_YOGA_LAYOUT_MANAGER_H_
#define CHROHIME_API_YOGA_LAYOUT_MANAGER_H_

#include "base/memory/raw_ptr.h"
#include "ui/views/layout/layout_manager.h"

namespace hime {

class View;

class YogaLayoutManager : public views::LayoutManager {
 public:
  explicit YogaLayoutManager(View* host);
  ~YogaLayoutManager() override;

  // views::LayoutManager:
  void Layout(views::View*) override;
  gfx::Size GetPreferredSize(const views::View*) const override;

 private:
  void UpdateChildBounds();

  bool dirty_ = false;
  raw_ptr<View> host_;
};

}  // namespace hime

#endif  // CHROHIME_API_YOGA_LAYOUT_MANAGER_H_
