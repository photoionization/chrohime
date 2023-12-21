// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_WEB_VIEW_H_
#define CHROHIME_API_WEB_VIEW_H_

#include "chrohime/api/view.h"
#include "content/public/browser/web_contents_delegate.h"
#include "ui/views/controls/webview/unhandled_keyboard_event_handler.h"
#include "url/gurl.h"

namespace views {
class WebView;
}

namespace hime {

class CHROHIME_EXPORT WebView : public View,
                                public content::WebContentsDelegate {
 public:
  WebView();

  void LoadInitialUrl(const GURL& url);

  views::WebView* GetView() const;

 protected:
  ~WebView() override;

  // content::WebContentsDelegate:
  bool HandleKeyboardEvent(
      content::WebContents* source,
      const content::NativeWebKeyboardEvent& event) override;

 private:
  views::UnhandledKeyboardEventHandler unhandled_keyboard_event_handler_;
};

}  // namespace hime

#endif  // CHROHIME_API_WEB_VIEW_H_
