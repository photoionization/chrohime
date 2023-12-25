// Copyright 2018 The Chromium Authors
// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include <optional>
#include <string>
#include <vector>

namespace hime {

// Creates the main menu bar using the name specified in |product_name|, and
// registers it as such on |nsapp|. The NSApplicationDelegate |app_delegate|
// is the target for specific, special menu items.
//
// Normally the main menu is built in a MainMenu.nib file, but NIB files files
// are hard to edit (especially cross-platform) and bring in a compile
// dependency on ibtool. Building the menu in code has a lower maintenance
// burden.
void BuildMainMenu(NSApplication* nsapp,
                   id<NSApplicationDelegate> app_delegate,
                   const std::u16string& product_name);

}  // namespace hime
