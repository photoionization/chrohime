// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_YOGA_UTIL_H_
#define CHROHIME_API_YOGA_UTIL_H_

#include <string>

typedef struct YGNode *YGNodeRef;

namespace hime {

void SetYogaProperty(YGNodeRef node, std::u16string_view key, float value);
void SetYogaProperty(YGNodeRef node,
                     std::u16string_view key,
                     std::u16string_view value);

}  // namespace hime

#endif  // CHROHIME_API_YOGA_UTIL_H_
