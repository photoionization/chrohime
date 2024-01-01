// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/event.h"

#include "base/strings/utf_string_conversions.h"
#include "ui/events/keycodes/dom/keycode_converter.h"

namespace hime {

KeyEvent::KeyEvent(ui::KeyEvent* event) : Event(event) {}

KeyEvent::~KeyEvent() = default;

const std::u16string& KeyEvent::GetKey() const {
  return cached_key_ = base::UTF8ToUTF16(
      ui::KeycodeConverter::DomKeyToKeyString(event()->GetDomKey()));
}

const std::u16string& KeyEvent::GetCode() const {
  return cached_code_ = base::UTF8ToUTF16(
      ui::KeycodeConverter::DomCodeToCodeString(event()->code()));
}

}  // namespace hime
