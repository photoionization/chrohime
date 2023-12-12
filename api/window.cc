// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/window.h"

#include "chrohime/api/root_view.h"
#include "chrohime/api/view.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/geometry/size.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_delegate.h"

#include "ui/views/controls/label.h"

namespace hime {

// The WindowDelegate is its own class because Window is a RefCounted and
// somehow RefCounted can not inherit from WidgetDelegateView.
class WindowDelegate : public views::WidgetDelegate {
 public:
  explicit WindowDelegate(Window* window) : window_(window) {
    RegisterDeleteDelegateCallback(base::BindOnce([](WindowDelegate* self) {
        self->window_->on_close.Emit(self->window_.get());
    }, base::Unretained(this)));
  }

  ~WindowDelegate() override = default;

  // views::WidgetDelegateView:
  bool CanResize() const override {
    return true;
  }

  std::u16string GetWindowTitle() const override {
    return window_->GetTitle();
  }

  views::View* GetContentsView() override {
    return window_->root_view_.get();
  }

 private:
  raw_ptr<Window> window_;
};

Window::Window(const Options& options)
    : root_view_(new RootView(this)),
      delegate_(new WindowDelegate(this)),
      widget_(new views::Widget) {
  views::Widget::InitParams params;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.delegate = delegate_.get();
  params.opacity = options.transparent ?
      views::Widget::InitParams::WindowOpacity::kTranslucent :
      views::Widget::InitParams::WindowOpacity::kOpaque;
  widget_->Init(std::move(params));
  SetContentView(base::MakeRefCounted<View>());
}

Window::~Window() = default;

void Window::Close() {
  widget_->Close();
}

void Window::Activate() {
  widget_->Show();
}

void Window::SetContentView(scoped_refptr<View> view) {
  if (content_view_)
    root_view_->RemoveChildView(content_view_->view());
  content_view_ = std::move(view);
  root_view_->AddChildView(content_view_->TransferOwnership());
  root_view_->Layout();
}

View* Window::GetContentView() const {
  return content_view_.get();
}

void Window::SetBounds(const gfx::Rect& bounds) {
  widget_->SetBounds(bounds);
}

gfx::Rect Window::GetBounds() const {
  return widget_->GetWindowBoundsInScreen();
}

void Window::SetSize(const gfx::Size& size) {
  widget_->SetSize(size);
}

gfx::Size Window::GetSize() const {
  return widget_->GetSize();
}

void Window::CenterTo(const gfx::Size& size) {
  widget_->CenterWindow(size);
}

void Window::SetTitle(std::u16string title) {
  title_ = std::move(title);
  widget_->UpdateWindowTitle();
}

const std::u16string& Window::GetTitle() const {
  return title_;
}

}  // namespace hime
