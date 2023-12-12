// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_SIGNAL_H_
#define CHROHIME_API_SIGNAL_H_

#include <algorithm>
#include <utility>
#include <vector>

#include "base/check.h"
#include "base/functional/callback.h"
#include "chrohime/chrohime_export.h"

namespace hime {

class SignalDelegate {
 public:
  virtual ~SignalDelegate() {}
  virtual void OnConnect(int32_t identifier) {}
};

// A simple signal/slot implementation.
template<typename Sig> class SignalBase {
 public:
  using Slot = base::RepeatingCallback<Sig>;

  void SetDelegate(SignalDelegate* delegate, int32_t identifier = 0) {
    delegate_ = delegate;
    identifier_ = identifier;
  }

  int32_t Connect(Slot slot) {
    CHECK(slot);
    if (delegate_ && slots_.empty())
      delegate_->OnConnect(identifier_);
    slots_.push_back(std::make_pair(++next_id_, std::move(slot)));
    return next_id_;
  }

  void Disconnect(int32_t id) {
    auto iter = std::lower_bound(slots_.begin(), slots_.end(),
                                 id, TupleCompare);
    if (iter != slots_.end() && std::get<0>(*iter) == id)
      slots_.erase(iter);
  }

  void DisconnectAll() {
    slots_.clear();
  }

  bool IsEmpty() const {
    return slots_.empty();
  }

 protected:
  // Use the first element of tuple as comparing key.
  static bool TupleCompare(const std::pair<int32_t, Slot>& element,
                           int32_t key) {
    return element.first < key;
  }

  int32_t next_id_ = 0;
  std::vector<std::pair<int32_t, Slot>> slots_;

  int32_t identifier_ = 0;
  raw_ptr<SignalDelegate> delegate_ = nullptr;
};

template<typename Sig> class Signal;

// Signal type that does not expect return type.
template<typename... Args>
class Signal<void(Args...)> : public SignalBase<void(Args...)> {
 public:
  using Base = SignalBase<void(Args...)>;

  template<typename... EmitArgs>
  void Emit(EmitArgs&&... args) {
    // Copy the list before iterating, since it is possible that user removes
    // elements from the list when iterating.
    auto slots = this->slots_;
    for (auto& slot : slots)
      slot.second.Run(std::forward<EmitArgs>(args)...);
  }
};

// Signal that expects boolean return value.
template<typename... Args>
class Signal<bool(Args...)> : public SignalBase<bool(Args...)> {
 public:
  using Base = SignalBase<bool(Args...)>;

  template<typename... EmitArgs>
  bool Emit(EmitArgs&&... args) {
    // Copy the list before iterating, since it is possible that user removes
    // elements from the list when iterating.
    auto slots = this->slots_;
    for (auto& slot : slots) {
      if (slot.second.Run(std::forward<EmitArgs>(args)...))
        return true;
    }
    return false;
  }
};

}  // namespace hime

#endif  // CHROHIME_API_SIGNAL_H_
