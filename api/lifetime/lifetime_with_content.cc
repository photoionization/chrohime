// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrohime/api/lifetime.h"

#include "base/command_line.h"
#include "base/files/scoped_temp_dir.h"
#include "chrohime/api/lifetime/chrohime_views_delegate.h"
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
#include "content/public/browser/context_factory.h"
#include "sandbox/mac/seatbelt_exec.h"
#endif

namespace hime {

class LifetimeImpl : public ContentLifetimeDelegate {
 public:
#if BUILDFLAG(IS_WIN)
  explicit LifetimeImpl(Lifetime* lifetime) : lifetime_(lifetime) {
    // Making sandbox work on Windows requires some non-simple work on user's
    // side, so disable sandbox for now. See CEF's comment on requirements:
    // https://github.com/chromiumembedded/cef/blob/master/include/cef_sandbox_win.h
    base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
    command_line->AppendSwitch(sandbox::policy::switches::kNoSandbox);
    // The SandboxInterfaceInfo is required by content.
    content::InitializeSandboxInfo(&sandbox_info_);
  }
#else  // BUILDFLAG(IS_WIN)
  LifetimeImpl(Lifetime* lifetime, int argc, const char** argv)
      : lifetime_(lifetime), argc_(argc), argv_(argv) {
#if BUILDFLAG(IS_MAC)
    base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
    sandbox::SeatbeltExecServer::CreateFromArgumentsResult seatbelt =
        sandbox::SeatbeltExecServer::CreateFromArguments(
            command_line->GetProgram().value().c_str(), argc,
            const_cast<char**>(argv));
    if (seatbelt.sandbox_required)
      CHECK(seatbelt.server->InitializeSandbox());
#endif  // BUILDFLAG(IS_MAC)
  }
#endif  // !BUILDFLAG(IS_WIN)

  int RunMain() {
    ChrohimeContentMainDelegate delegate(lifetime_);
    content::ContentMainParams params(&delegate);
#if BUILDFLAG(IS_WIN)
    params.sandbox_info = &sandbox_info_;
#else
    params.argc = argc_;
    params.argv = argv_;
#endif
    return content::ContentMain(std::move(params));
  }

  // ContentLifetimeDelegate:
  void OnPreMainMessageLoopRun() override {
#if BUILDFLAG(IS_MAC)
    lifetime_->views_delegate()->set_context_factory(
        content::GetContextFactory());
#endif
    std::ignore = temp_dir_.CreateUniqueTempDir();
    browser_context_ = std::make_unique<webui_examples::BrowserContext>(
        temp_dir_.GetPath());
    State::GetCurrent()->set_browser_context(browser_context_.get());
#if !BUILDFLAG(IS_MAC)
    lifetime_->on_ready.Emit();
#endif
  }

  void OnPostMainMessageLoopRun() override {
    browser_context_.reset();
  }

 private:
  raw_ptr<Lifetime> lifetime_;
#if BUILDFLAG(IS_WIN)
  sandbox::SandboxInterfaceInfo sandbox_info_{nullptr};
#else
  int argc_ = 0;
  raw_ptr<const char*> argv_ = nullptr;
#endif

  base::ScopedTempDir temp_dir_;
  std::unique_ptr<webui_examples::BrowserContext> browser_context_;
};

int Lifetime::RunMain() {
  return static_cast<LifetimeImpl*>(impl_)->RunMain();
}

#if BUILDFLAG(IS_WIN)
void Lifetime::Initialize() {
  impl_ = new LifetimeImpl(this);
#else
void Lifetime::Initialize(int argc, const char** argv) {
  impl_ = new LifetimeImpl(this, argc, argv);
#endif
}

void Lifetime::Destroy() {
  delete impl_.get();
}

}  // namespace hime
