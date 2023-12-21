// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime/chrohime_app_delegate.h"

#include "chrohime/api/lifetime.h"

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
  NSMenu* mainMenu = [[NSMenu alloc] initWithTitle:@""];
  NSMenuItem* appMenuItem = [mainMenu addItemWithTitle:@""
                                                action:nullptr
                                         keyEquivalent:@""];
  NSMenu* appMenu = [[NSMenu alloc] initWithTitle:@""];
  NSString* appName = NSProcessInfo.processInfo.processName;
  NSString* quitTitle = [@"Quit " stringByAppendingString:appName];
  [appMenu addItemWithTitle:quitTitle
                     action:@selector(terminate:)
              keyEquivalent:@"q"];
  [appMenuItem setSubmenu:appMenu];

  [NSApp setMainMenu:mainMenu];

  shell_->on_ready.Emit();
}

@end
