// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_OBJECT_H_
#define CHROHIME_API_OBJECT_H_

#include "base/memory/ref_counted.h"
#include "chrohime/chrohime_export.h"

namespace hime {

class CHROHIME_EXPORT Object : public base::RefCounted<Object> {
 public:
  // The reference count starts from 1, so we don't have concepts like floating
  // reference. This is because we assume this library is mostly used by
  // language bindings.
  REQUIRE_ADOPTION_FOR_REFCOUNTED_TYPE();

  void Ref();
  void Unref();

 protected:
  Object();
  virtual ~Object();

  friend class base::RefCounted<Object>;
};

}  // namespace hime

#endif  // CHROHIME_API_OBJECT_H_
