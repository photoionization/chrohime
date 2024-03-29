// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/content/chrohime_content_main_delegate.h"

#include "base/files/file_path.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "chrohime/content/chrohime_content_browser_client.h"
#include "ui/base/resource/resource_bundle.h"

namespace hime {

ChrohimeContentMainDelegate::ChrohimeContentMainDelegate(
    ContentLifetimeDelegate* delegate) : delegate_(delegate) {}

ChrohimeContentMainDelegate::~ChrohimeContentMainDelegate() = default;

std::optional<int> ChrohimeContentMainDelegate::BasicStartupComplete() {
  logging::LoggingSettings settings;
  settings.logging_dest = logging::LOG_TO_SYSTEM_DEBUG_LOG |
                          logging::LOG_TO_STDERR;
  logging::InitLogging(settings);
  return std::nullopt;
}

void ChrohimeContentMainDelegate::PreSandboxStartup() {
  base::FilePath resources_pak_path;
  base::PathService::Get(base::DIR_ASSETS, &resources_pak_path);
  ui::ResourceBundle::InitSharedInstanceWithPakPath(
      resources_pak_path.AppendASCII("chrohime_with_content.pak"));
}

content::ContentClient* ChrohimeContentMainDelegate::CreateContentClient() {
  return &content_client_;
}

content::ContentBrowserClient*
ChrohimeContentMainDelegate::CreateContentBrowserClient() {
  browser_client_ = std::make_unique<ChrohimeContentBrowserClient>(
      delegate_.get());
  return browser_client_.get();
}

}  // namespace hime
