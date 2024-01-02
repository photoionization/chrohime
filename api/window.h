// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_WINDOW_H_
#define CHROHIME_API_WINDOW_H_

#include "chrohime/api/object.h"
#include "chrohime/api/signal.h"
#include "ui/views/widget/widget_observer.h"

namespace gfx {
class Size;
class SizeF;
}

namespace hime {

class RootView;
class View;
class WindowDelegate;

class CHROHIME_EXPORT Window : public Object,
                               public views::WidgetObserver {
 public:
  struct Options {
    bool transparent = false;
  };

  explicit Window(const Options& options);

  void Close();

  void SetContentView(scoped_refptr<View> view);
  View* GetContentView() const;

  void Activate();
  void Deactivate();
  bool IsActive() const;
  void SetVisible(bool visible);
  bool IsVisible() const;
  void SetFullscreen(bool fullscreen);
  bool IsFullscreen() const;
  void Maximize();
  bool IsMaximized() const;
  void Minimize();
  bool IsMinimized() const;
  void Restore();

  void SetResizable(bool resizable);
  bool IsResizable() const;
  void SetMaximizable(bool maximizable);
  bool IsMaximizable() const;
  void SetMinimizable(bool minimizable);
  bool IsMinimizable() const;
  void SetFullscreenable(bool fullscreenable);
  bool IsFullscreenable() const;

  void SetBounds(const gfx::Rect& bounds);
  gfx::Rect GetBounds() const;
  void SetSize(const gfx::Size& size);
  gfx::Size GetSize() const;
  void CenterTo(const gfx::Size& size);

  void SetTitle(std::u16string title);
  const std::u16string& GetTitle() const;
  void SetOpacity(float opacity);
  void SetAspectRatio(const gfx::SizeF& ratio);
#if BUILDFLAG(IS_WIN)
  void FlashFrame(bool flash);
#endif

  // Events.
  Signal<void(Window*)> on_close;
  Signal<void(Window*, bool)> on_visibility_change;
  Signal<void(Window*, bool)> on_activation_change;
  Signal<void(Window*, const gfx::Rect&)> on_bounds_change;

 protected:
  friend class WindowDelegate;

  ~Window() override;

  // views::WidgetObserver:
  void OnWidgetVisibilityChanged(views::Widget* widget, bool visible) override;
  void OnWidgetActivationChanged(views::Widget* widget, bool active) override;
  void OnWidgetBoundsChanged(views::Widget* widget,
                             const gfx::Rect& new_bounds) override;

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
