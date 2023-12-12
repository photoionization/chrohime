// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_WINDOW_H_
#define CHROHIME_API_WINDOW_H_

#include "chrohime/api/object.h"
#include "chrohime/api/signal.h"

namespace gfx {
class Rect;
class Size;
}

namespace views {
class Widget;
}

namespace hime {

class RootView;
class View;
class WindowDelegate;

class CHROHIME_EXPORT Window : public Object {
 public:
  struct Options {
    bool transparent = false;
  };

  explicit Window(const Options& options);

  void Close();
  void Activate();

  void SetContentView(scoped_refptr<View> view);
  View* GetContentView() const;

  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds() const;
  void SetSize(const gfx::Size& size);
  gfx::Size GetSize() const;
  void CenterTo(const gfx::Size& size);

  void SetTitle(std::u16string title);
  const std::u16string& GetTitle() const;

  // Events.
  Signal<void(Window*)> on_close;

 protected:
  friend class WindowDelegate;

  ~Window() override;

 private:
  bool is_closing_ = false;
  std::u16string title_;
  scoped_refptr<View> content_view_;
  std::unique_ptr<RootView> root_view_;
  std::unique_ptr<WindowDelegate> delegate_;
  std::unique_ptr<views::Widget> widget_;
};

}  // namespace hime

#endif  // CHROHIME_API_WINDOW_H_
