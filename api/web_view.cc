// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/web_view.h"

#include "chrohime/api/state.h"
#include "ui/views/controls/webview/webview.h"

namespace hime {

WebView::WebView()
    : View(std::make_unique<views::WebView>(
               State::GetCurrent()->GetBrowserContext()),
           LayoutType::kLeaf) {
  GetView()->GetWebContents()->SetDelegate(this);
}

WebView::~WebView() = default;

void WebView::LoadInitialUrl(const GURL& url) {
  HIME_RETURN_ON_DESTROYED_VIEW(this);
  GetView()->LoadInitialURL(url);
}

views::WebView* WebView::GetView() const {
  return static_cast<views::WebView*>(view());
}

bool WebView::HandleKeyboardEvent(
    content::WebContents* source,
    const content::NativeWebKeyboardEvent& event) {
  return unhandled_keyboard_event_handler_.HandleKeyboardEvent(
      event, GetView()->GetFocusManager());
}

}  // namespace hime
