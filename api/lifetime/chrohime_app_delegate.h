// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LIFETIME_CHROHIME_APP_DELEGATE_H_
#define CHROHIME_API_LIFETIME_CHROHIME_APP_DELEGATE_H_

#include "base/memory/raw_ptr.h"

#import <Cocoa/Cocoa.h>

namespace hime {
class Lifetime;
}

@interface ChrohimeAppDelegate : NSObject<NSApplicationDelegate> {
 @private
  raw_ptr<hime::Lifetime> shell_;
}
- (id)initWithShell:(hime::Lifetime*)shell;
@end

#endif  // CHROHIME_API_LIFETIME_CHROHIME_APP_DELEGATE_H_
