// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/object.h"

namespace hime {

Object::Object() = default;

Object::~Object() = default;

void Object::Ref() {
  AddRef();
}

void Object::Unref() {
  Release();
}

}  // namespace hime
