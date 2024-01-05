// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_BUFFER_H_
#define CHROHIME_API_BUFFER_H_

#include <functional>

#include "base/memory/raw_ptr.h"
#include "chrohime/chrohime_export.h"

namespace hime {

// A move-only class to manage memory.
// Note: Currently for language bindings we are assuming the buffer passed to
// APIs are consumed immediately, so memory passed from language bindings are
// NOT copied. Keep this in mind when designing new APIs.
class CHROHIME_EXPORT Buffer {
 public:
  using FreeFunc = std::function<void(void*)>;

  // Wrap the memory but do not free it.
  static Buffer Wrap(const void* content, size_t size);

  // Take over the memory and free it when done.
  static Buffer TakeOver(void* content, size_t size, FreeFunc free);

  Buffer(Buffer&& other) noexcept;
  Buffer() noexcept;

  ~Buffer();

  Buffer& operator=(Buffer&& other) noexcept;
  Buffer& operator=(const Buffer&) = delete;
  Buffer(const Buffer&) = delete;

  void* content() const { return content_; }
  size_t size() const { return size_; }

 private:
  Buffer(void* content, size_t size, FreeFunc free);

  raw_ptr<void> content_ = nullptr;
  size_t size_ = 0;
  FreeFunc free_;
};

}  // namespace hime

#endif  // CHROHIME_API_BUFFER_H_
