// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime.h"

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/functional/bind.h"
#include "chrohime/api/state.h"
#include "chrohime/content/chrohime_content_main_delegate.h"
#include "content/public/app/content_main.h"
#include "ui/webui/examples/browser/browser_context.h"

#if BUILDFLAG(IS_WIN)
#include "content/public/app/sandbox_helper_win.h"
#include "sandbox/policy/switches.h"
#include "sandbox/win/src/sandbox_types.h"
#endif

#if BUILDFLAG(IS_MAC)
#include "sandbox/mac/seatbelt_exec.h"
#endif

namespace hime {

struct LifetimeImpl {
  base::ScopedTempDir temp_dir;
  std::unique_ptr<webui_examples::BrowserContext> browser_context;
#if BUILDFLAG(IS_WIN)
  sandbox::SandboxInterfaceInfo sandbox_info{nullptr};
#else
  int argc = 0;
  raw_ptr<const char*> argv = nullptr;
#endif
};

int Lifetime::RunMain() {
  ChrohimeContentMainDelegate delegate(this);
  content::ContentMainParams params(&delegate);
#if BUILDFLAG(IS_WIN)
  params.sandbox_info = &impl_->sandbox_info;
#else
  params.argc = impl_->argc;
  params.argv = impl_->argv;
#endif
  return content::ContentMain(std::move(params));
}

void Lifetime::OnPreMainMessageLoopRun(base::RepeatingClosure quit_closure) {
  std::ignore = impl_->temp_dir.CreateUniqueTempDir();
  impl_->browser_context = std::make_unique<webui_examples::BrowserContext>(
      impl_->temp_dir.GetPath());
  State::GetCurrent()->browser_context_ = impl_->browser_context.get();
  quit_closure_ = std::move(quit_closure);
#if !BUILDFLAG(IS_MAC)
  on_ready.Emit();
#endif
}

void Lifetime::OnPostMainMessageLoopRun() {
  impl_->browser_context.reset();
}

#if BUILDFLAG(IS_WIN)
void Lifetime::Initialize() {
#else
void Lifetime::Initialize(int argc, const char** argv) {
#endif
  impl_ = new LifetimeImpl;

#if BUILDFLAG(IS_WIN)
  // Making sandbox work on Windows requires some non-simple work on user's
  // side, so disable sandbox for now. See CEF's comment on requirements:
  // https://github.com/chromiumembedded/cef/blob/master/include/cef_sandbox_win.h
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  command_line->AppendSwitch(sandbox::policy::switches::kNoSandbox);
  // The SandboxInterfaceInfo is required by content.
  content::InitializeSandboxInfo(&impl_->sandbox_info);
#elif BUILDFLAG(IS_MAC)
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  sandbox::SeatbeltExecServer::CreateFromArgumentsResult seatbelt =
      sandbox::SeatbeltExecServer::CreateFromArguments(
          command_line->GetProgram().value().c_str(), argc,
          const_cast<char**>(argv));
  if (seatbelt.sandbox_required)
    CHECK(seatbelt.server->InitializeSandbox());
#endif

#if !BUILDFLAG(IS_WIN)
  impl_->argc = argc;
  impl_->argv = argv;
#endif
}

void Lifetime::Destroy() {
  delete impl_.get();
}

}  // namespace hime
