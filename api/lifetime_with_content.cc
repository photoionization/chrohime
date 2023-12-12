// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime.h"

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/functional/bind.h"
#include "base/path_service.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/views_content_client/views_content_client.h"

#if BUILDFLAG(IS_WIN)
#include "base/win/windows_types.h"
#include "content/public/app/sandbox_helper_win.h"
#include "sandbox/win/src/sandbox_types.h"
#endif

#if BUILDFLAG(IS_MAC)
#include "sandbox/mac/seatbelt_exec.h"
#endif

namespace hime {

namespace {

void OnResourcesLoaded() {
  base::FilePath resources_pak_path;
  CHECK(base::PathService::Get(base::DIR_ASSETS, &resources_pak_path));
  ui::ResourceBundle::GetSharedInstance().AddDataPackFromPath(
      resources_pak_path.AppendASCII("chrohime_resources.pak"),
      ui::k100Percent);
}

}  // namespace

struct LifetimeImpl {
  std::unique_ptr<ui::ViewsContentClient> content_client;
};

int Lifetime::RunMain() {
  return impl_->content_client->RunMain();
}

#if BUILDFLAG(IS_WIN)
void Lifetime::Initialize() {
#else
void Lifetime::Initialize(int argc, const char** argv) {
#endif
  impl_ = new LifetimeImpl;

#if BUILDFLAG(IS_WIN)
  sandbox::SandboxInterfaceInfo sandbox_info = {nullptr};
  content::InitializeSandboxInfo(&sandbox_info);
  impl_->content_client.reset(
      new ui::ViewsContentClient(instance, &sandbox_info));
#elif BUILDFLAG(IS_MAC)
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  // ViewsContentClient expects a const char** argv and
  // CreateFromArgumentsResult expects a regular char** argv. Given this is a
  // test program, a refactor from either end didn't seem worth it. As a result,
  // use a const_cast instead.
  sandbox::SeatbeltExecServer::CreateFromArgumentsResult seatbelt =
      sandbox::SeatbeltExecServer::CreateFromArguments(
          command_line->GetProgram().value().c_str(), argc,
          const_cast<char**>(argv));
  if (seatbelt.sandbox_required)
    CHECK(seatbelt.server->InitializeSandbox());
#endif

  impl_->content_client.reset(new ui::ViewsContentClient(argc, argv));
  impl_->content_client->set_on_resources_loaded_callback(
      base::BindOnce(&OnResourcesLoaded));
  impl_->content_client->set_on_pre_main_message_loop_run_callback(
      base::BindOnce([](base::WeakPtr<Lifetime> self,
                        content::BrowserContext*, gfx::NativeWindow) {
        self->on_ready.Emit();
      }, GetWeakPtr()));
}

void Lifetime::Destroy() {
  delete impl_.get();
}

}  // namespace hime
