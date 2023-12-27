// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_LIFETIME_CHROHIME_APP_H_
#define CHROHIME_API_LIFETIME_CHROHIME_APP_H_

#include "base/mac/scoped_sending_event.h"
#include "base/message_loop/message_pump_apple.h"

@interface ChrohimeApplication : NSApplication<CrAppProtocol,
                                               CrAppControlProtocol>
- (BOOL)isHandlingSendEvent;
- (void)setHandlingSendEvent:(BOOL)handlingSendEvent;
@end

#endif  // CHROHIME_API_LIFETIME_CHROHIME_APP_H_
