// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/web_view.h"

#include "chrohime/api/state.h"
#include "chrohime/api/view_event_dispatcher.h"
#include "ui/views/controls/webview/webview.h"

namespace hime {

namespace {

class WebViewImpl : public ViewEventDispatcher<hime::WebView,
                                               views::WebView> {
 public:
  WebViewImpl(hime::WebView* delegate, content::BrowserContext* browser_context)
      : ViewBaseDispatcher(delegate, browser_context) {}
};

}  // namespace

WebView::WebView()
    : View(std::make_unique<WebViewImpl>(
               this, State::GetCurrent()->GetBrowserContext()),
           LayoutType::kLeaf) {
  GetView()->GetWebContents()->SetDelegate(this);
}

WebView::~WebView() = default;

void WebView::LoadInitialUrl(const GURL& url) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->LoadInitialURL(url);
}

bool WebView::HandleKeyboardEvent(
    content::WebContents* source,
    const content::NativeWebKeyboardEvent& event) {
  return unhandled_keyboard_event_handler_.HandleKeyboardEvent(
      event, GetView()->GetFocusManager());
}

views::WebView* WebView::GetView() const {
  return static_cast<views::WebView*>(view());
}

}  // namespace hime
