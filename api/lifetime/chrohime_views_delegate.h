// Copyright 2014 The Chromium Authors
// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LIFETIME_CHROHIME_VIEWS_DELEGATE_H_
#define CHROHIME_API_LIFETIME_CHROHIME_VIEWS_DELEGATE_H_

#include "ui/views/layout/layout_provider.h"
#include "ui/views/views_delegate.h"

namespace hime {

class ChrohimeViewsDelegate : public views::ViewsDelegate {
 public:
  ChrohimeViewsDelegate();
  ~ChrohimeViewsDelegate() override;

#if BUILDFLAG(IS_MAC)
  void set_context_factory(ui::ContextFactory* context_factory) {
    context_factory_ = context_factory;
  }
#endif

  // content::ViewsDelegate:
#if BUILDFLAG(IS_WIN)
  HICON GetSmallWindowIcon() const override;
#endif
  void OnBeforeWidgetInit(
      views::Widget::InitParams* params,
      views::internal::NativeWidgetDelegate* delegate) override;
#if BUILDFLAG(IS_MAC)
  ui::ContextFactory* GetContextFactory() override;
#endif

 private:
  views::LayoutProvider layout_provider_;
#if BUILDFLAG(IS_MAC)
  raw_ptr<ui::ContextFactory> context_factory_ = nullptr;
#endif
};

}  // namespace hime

#endif  // CHROHIME_API_LIFETIME_CHROHIME_VIEWS_DELEGATE_H_
