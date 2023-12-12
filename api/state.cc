// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/state.h"

#include <iostream>

#include "base/check.h"

namespace hime {

namespace {

thread_local State* g_state = nullptr;

}  // namespace

// static
State* State::GetCurrent() {
  CHECK(g_state) << "Must initialize State before using any chrohime API.";
  return g_state;
}

State::State() {
  g_state = this;
}

State::~State() {
  g_state = nullptr;
}

View* State::GetViewFromViewsView(views::View* view) const {
  auto it = views_map_.find(view);
  if (it == views_map_.end())
    return nullptr;
  return it->second;
}

}  // namespace hime
