// Copyright 2014 The Chromium Authors
// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/functional/bind.h"
#include "chrohime/api/chrohime.h"
#include "ui/gfx/geometry/size.h"

#if BUILDFLAG(IS_WIN)
int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, wchar_t*, int) {
  hime::Lifetime lifetime;
#else
int main(int argc, const char** argv) {
  hime::Lifetime lifetime(argc, argv);
#endif

  hime::State state;

  lifetime.on_ready.Connect(base::BindRepeating([]() {
    auto win = base::AdoptRef(new hime::Window({}));
    win->SetTitle(u"👸🏾");
    win->CenterTo(gfx::Size(400, 300));
    auto label = base::AdoptRef(new hime::Label);
    label->SetText(u"Hello World");
    win->SetContentView(std::move(label));
    win->Activate();
    // Move the ownership to on_close event, so window is released on close.
    win->on_close.Connect(base::BindRepeating([](scoped_refptr<hime::Window>,
                                                 hime::Window* win) {
      // Do nothing and let the scoped_refptr do the cleanup.
    }, std::move(win)));
  }));
  return lifetime.RunMain();
}
