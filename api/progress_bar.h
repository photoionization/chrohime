// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_PROGRESS_BAR_H_
#define CHROHIME_API_PROGRESS_BAR_H_

#include "chrohime/api/view.h"

namespace views {
class ProgressBar;
}

namespace hime {

class CHROHIME_EXPORT ProgressBar : public View {
 public:
  ProgressBar();

  void SetValue(float value);
  float GetValue() const;
  void SetIndeterminate(bool indeterminate);
  bool IsIndeterminate() const;

  views::ProgressBar* GetView() const;

 protected:
  ~ProgressBar() override;
};

}  // namespace hime

#endif  // CHROHIME_API_PROGRESS_BAR_H_
