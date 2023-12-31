// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_API_COMBOBOX_MODEL_H_
#define CHROHIME_API_COMBOBOX_MODEL_H_

#include "chrohime/api/object.h"
#include "ui/base/models/combobox_model_observer.h"

namespace ui {
class ComboboxModel;
}

namespace hime {

class CHROHIME_EXPORT ComboboxModel : public Object,
                                      public ui::ComboboxModelObserver {
 public:
  size_t GetItemCount() const;
  const std::u16string& GetItemText(size_t index) const;

  // Internal: Give the ownership of underlying ui::ComboboxModel to caller.
  std::unique_ptr<ui::ComboboxModel> TransferOwnership();

 protected:
  explicit ComboboxModel(std::unique_ptr<ui::ComboboxModel> to_take);
  ~ComboboxModel() override;

  // ui::ComboboxModelObserver:
  void OnComboboxModelChanged(ui::ComboboxModel* model) override;
  void OnComboboxModelDestroying(ui::ComboboxModel* model) override;

  ui::ComboboxModel* model() const { return model_.get(); }

 private:
  // Cache the result so we can return const string reference in APIs.
  mutable std::u16string cached_result_;

  raw_ptr<ui::ComboboxModel> model_ = nullptr;
  std::unique_ptr<ui::ComboboxModel> ownership_;
};

// Helper for guarding against destroyed models.
#define HIME_RETURN_ON_DESTROYED_COMBOBOX_MODEL(obj) \
  if (!obj || !obj->model()) { \
    NOTREACHED() << "ComboboxModel has been destroyed."; \
    return; \
  }
#define HIME_RETURN_VALUE_ON_DESTROYED_COMBOBOX_MODEL(obj, value) \
  if (!obj || !obj->model()) { \
    NOTREACHED() << "ComboboxModel has been destroyed."; \
    return value; \
  }

}  // namespace hime

#endif  // CHROHIME_API_COMBOBOX_MODEL_H_
