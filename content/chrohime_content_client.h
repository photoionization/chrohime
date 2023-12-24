// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CONTENT_CHROHIME_CONTENT_CLIENT_H_
#define CHROHIME_CONTENT_CHROHIME_CONTENT_CLIENT_H_

#include "chrohime/chrohime_export.h"
#include "content/public/common/content_client.h"

namespace hime {

class ChrohimeContentClient : public content::ContentClient {
 public:
  ChrohimeContentClient();
  ~ChrohimeContentClient() override;

  std::u16string GetLocalizedString(int message_id) override;
  base::StringPiece GetDataResource(
      int resource_id,
      ui::ResourceScaleFactor scale_factor) override;
  base::RefCountedMemory* GetDataResourceBytes(int resource_id) override;
  std::string GetDataResourceString(int resource_id) override;
  gfx::Image& GetNativeImageNamed(int resource_id) override;
};

}  // namespace hime

#endif  // CHROHIME_CONTENT_CHROHIME_CONTENT_CLIENT_H_
