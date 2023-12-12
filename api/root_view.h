// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_ROOT_VIEW_H_
#define CHROHIME_API_ROOT_VIEW_H_

#include "ui/views/view.h"

namespace hime {

class Window;

// The views::Widget does not allow changing content view dynamically, so we
// use the RootView as the widget's real content view, and implement the content
// view set by |Window::SetContentView| as a child view of RootView.
class RootView : public views::View {
 public:
  explicit RootView(Window* window);
  ~RootView() override;

  // views::View:
  void Layout() override;

 private:
  raw_ptr<Window> window_;
};

}  // namespace hime

#endif  // CHROHIME_API_ROOT_VIEW_H_
