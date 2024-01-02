// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/window.h"

#include "base/task/single_thread_task_runner.h"
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
    SetCanFullscreen(true);
    SetCanMaximize(true);
    SetCanMinimize(true);
    SetCanResize(true);
  }

  ~WindowDelegate() override = default;

  // views::WidgetDelegateView:
  std::u16string GetWindowTitle() const override {
    return window_->GetTitle();
  }

  void WindowClosing() override {
    // Destroying the widget will crash now, so delay the on_close event
    // until next tick.
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE,
        base::BindOnce([](base::WeakPtr<WindowDelegate> self) {
          if (self)
            self->window_->on_close.Emit(self->window_.get());
        }, weak_factory_.GetWeakPtr()));
  }

  views::View* GetContentsView() override {
    return window_->root_view_.get();
  }

 private:
  raw_ptr<Window> window_;
  base::WeakPtrFactory<WindowDelegate> weak_factory_{this};
};

Window::Window(const Options& options)
    : root_view_(new RootView),
      delegate_(new WindowDelegate(this)),
      widget_(new views::Widget) {
  views::Widget::InitParams params;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  params.delegate = delegate_.get();
  params.opacity = options.transparent ?
      views::Widget::InitParams::WindowOpacity::kTranslucent :
      views::Widget::InitParams::WindowOpacity::kOpaque;

  widget_->AddObserver(this);
  widget_->Init(std::move(params));
  SetContentView(base::MakeRefCounted<View>());
}

Window::~Window() {
  widget_->RemoveObserver(this);
}

void Window::Close() {
  widget_->Close();
}

void Window::SetContentView(scoped_refptr<View> view) {
  content_view_ = std::move(view);
  root_view_->SetContentView(content_view_->TransferOwnership());
  root_view_->Layout();
}

View* Window::GetContentView() const {
  return content_view_.get();
}

void Window::Activate() {
  widget_->Show();
}

void Window::Deactivate() {
  widget_->Deactivate();
}

bool Window::IsActive() const {
  return widget_->IsActive();
}

void Window::SetVisible(bool visible) {
  if (visible)
    widget_->ShowInactive();
  else
    widget_->Hide();
}

bool Window::IsVisible() const {
  return widget_->IsVisible();
}

void Window::SetFullscreen(bool fullscreen) {
  widget_->SetFullscreen(fullscreen);
}

bool Window::IsFullscreen() const {
  return widget_->IsFullscreen();
}

void Window::Maximize() {
  widget_->Maximize();
}

bool Window::IsMaximized() const {
  return widget_->IsMaximized();
}

void Window::Minimize() {
  widget_->Minimize();
}

bool Window::IsMinimized() const {
  return widget_->IsMinimized();
}

void Window::Restore() {
  widget_->Restore();
}

void Window::SetResizable(bool resizable) {
  delegate_->SetCanResize(resizable);
}

bool Window::IsResizable() const {
  return delegate_->CanResize();
}

void Window::SetMaximizable(bool maximizable) {
  delegate_->SetCanMaximize(maximizable);
}

bool Window::IsMaximizable() const {
  return delegate_->CanMaximize();
}

void Window::SetMinimizable(bool minimizable) {
  delegate_->SetCanMinimize(minimizable);
}

bool Window::IsMinimizable() const {
  return delegate_->CanMinimize();
}

void Window::SetFullscreenable(bool fullscreenable) {
  delegate_->SetCanFullscreen(fullscreenable);
}

bool Window::IsFullscreenable() const {
  return delegate_->CanFullscreen();
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

void Window::SetOpacity(float opacity) {
  widget_->SetOpacity(opacity);
}

void Window::SetAspectRatio(const gfx::SizeF& ratio) {
  widget_->SetAspectRatio(ratio);
}

#if BUILDFLAG(IS_WIN)
void Window::FlashFrame(bool flash) {
  widget_->FlashFrame(flash);
}
#endif

void Window::OnWidgetVisibilityChanged(views::Widget* widget, bool visible) {
  on_visibility_change.Emit(this, visible);
}

void Window::OnWidgetActivationChanged(views::Widget* widget, bool active) {
  on_activation_change.Emit(this, active);
}

void Window::OnWidgetBoundsChanged(views::Widget* widget,
                                   const gfx::Rect& new_bounds) {
  on_bounds_change.Emit(this, new_bounds);
}

}  // namespace hime
