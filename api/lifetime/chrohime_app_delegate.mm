// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime/chrohime_app_delegate.h"

#include "base/strings/sys_string_conversions.h"
#include "chrohime/api/lifetime.h"
#include "chrohime/api/lifetime/main_menu_builder.h"

@implementation ChrohimeAppDelegate

- (id)initWithShell:(hime::Lifetime*)shell {
  if ((self = [super init]))
    shell_ = shell;
  return self;
}

- (void)applicationWillFinishLaunching:(NSNotification*)notify {
  // Don't add the "Enter Full Screen" menu item automatically.
  [[NSUserDefaults standardUserDefaults]
      setBool:NO
       forKey:@"NSFullScreenMenuItemEverywhere"];
}

- (void)applicationDidFinishLaunching:(NSNotification*)notify {
  // To get key events, the application needs to have an activation policy.
  // Unbundled apps (i.e. those without an Info.plist) default to
  // NSApplicationActivationPolicyProhibited.
  [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

  // Create a basic mainMenu object using the executable filename.
  NSString* appName = NSProcessInfo.processInfo.processName;
  hime::BuildMainMenu(NSApp, self, base::SysNSStringToUTF16(appName));

  shell_->on_ready.Emit();
}

@end
