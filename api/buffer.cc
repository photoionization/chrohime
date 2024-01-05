// Copyright 2024 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/buffer.h"

#include <iostream>

#include "base/check.h"

namespace hime {

// static
Buffer Buffer::Wrap(const void* content, size_t size) {
  return Buffer(const_cast<void*>(content), size, nullptr);
}

// static
Buffer Buffer::TakeOver(void* content, size_t size, FreeFunc free) {
  CHECK(free) << "Must pass a valid free function to TakeOver";
  return Buffer(content, size, std::move(free));
}

Buffer::Buffer(void* content, size_t size, FreeFunc free)
    : content_(content),
      size_(size),
      free_(std::move(free)) {
}

Buffer::Buffer(Buffer&& other) noexcept
    : content_(other.content_),
      size_(other.size_),
      free_(std::move(other.free_)) {
  other.content_ = nullptr;
  other.size_ = 0;
}

Buffer::Buffer() noexcept {
}

Buffer::~Buffer() {
  if (free_)
    free_(content_);
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  if (free_)
    free_(content_);
  content_ = other.content_;
  size_ = other.size_;
  free_ = std::move(other.free_);
  other.content_ = nullptr;
  other.size_ = 0;
  return *this;
}

}  // namespace hime
