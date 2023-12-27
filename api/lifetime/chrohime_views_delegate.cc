// Copyright 2014 The Chromium Authors
// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime/chrohime_views_delegate.h"

#include "ui/views/buildflags.h"

#if BUILDFLAG(ENABLE_DESKTOP_AURA)
#include "ui/views/widget/desktop_aura/desktop_native_widget_aura.h"
#include "ui/views/widget/native_widget_aura.h"
#endif

#if BUILDFLAG(IS_MAC)
#include "ui/views/widget/native_widget_mac.h"
#endif

namespace hime {

ChrohimeViewsDelegate::ChrohimeViewsDelegate() = default;

ChrohimeViewsDelegate::~ChrohimeViewsDelegate() = default;

#if BUILDFLAG(IS_WIN)
HICON ChrohimeViewsDelegate::GetSmallWindowIcon() const {
  return nullptr;
}
#endif

void ChrohimeViewsDelegate::OnBeforeWidgetInit(
    views::Widget::InitParams* params,
    views::internal::NativeWidgetDelegate* delegate) {
  if (params->native_widget)
    return;
#if BUILDFLAG(ENABLE_DESKTOP_AURA)
  if (params->parent && params->type != views::Widget::InitParams::TYPE_MENU &&
      params->type != views::Widget::InitParams::TYPE_TOOLTIP) {
    params->native_widget = new views::NativeWidgetAura(delegate);
  } else {
    params->native_widget = new views::DesktopNativeWidgetAura(delegate);
  }
#elif BUILDFLAG(IS_MAC)
  params->native_widget = new views::NativeWidgetMac(delegate);
#endif
}

#if BUILDFLAG(IS_MAC)
ui::ContextFactory* ChrohimeViewsDelegate::GetContextFactory() {
  return context_factory_;
}
#endif

}  // namespace hime
