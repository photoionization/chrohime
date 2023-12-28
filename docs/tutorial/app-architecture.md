# App architecture

Chrohime may be used in 2 environments:

1. An empty environment where everything has to be created from scratch. This is
   usually the case when you build apps with Chrohime's shared libraries.
2. A managed Chromium environemt, where things like run loops and app delegates
   are already set up. This is when using Chrohime APIs inside apps built by
   Electron or CEF.

There are 2 versions of Chrohime binaries:

1. The one without content module (i.e. Chromium's browser part). This version
   can only create native UI and does not have access to webview APIs.
2. The one with content module. This version has a few more APIs for Chromium's
   browser code, and has a much larger binary size.

In Chrohime we try to hide the complexity with simple APIs, but it is important
to understand what actually happens in different cases to write correct code.

## State

Chrohime allocates lots of global objects when using, and it is developer's
responsibility to prepare spaces for them. Before using any API, developer
should create a `State` object first, after done with Chrohime, it is
developer's choice whether to free the `State` object - for most cases it is
fine to leak on exit, but you might want to explicitly free it when running
tests.

```c
hime_state_t state = hime_state_create();
...
hime_state_destroy(state);
```

## Lifetime

When setting up an environment for `ui/view` library, there are lots of
preparation work to do, like creating run loops and initializing various
libraries.

In Chrohime, such work is done by the `Lifetime` object.

```c
int main(int argc, const char** argv) {
  hime_state_t state = hime_state_create();
#if defined(_WIN32)
  hime_lifetime_t lifetime = hime_lifetime_create();
#else
  hime_lifetime_t lifetime = hime_lifetime_create(argc, argv);
#endif
  ...
  hime_lifetime_destroy(lifetime);
  hime_state_destroy(state);
  ...
}
```

Note that you only need to use `Lifetime` API when writing an app from scratch,
this API is not available when using Chrohime inside Electron or CEF apps.

### Entry point

After creating a `Lifetime` object, you are not supposed to use other APIs
immediately, they can only be used after the `ready` event.

```c
void OnReady(void* data) {
  hime_window_options_t options;
  hime_window_options_init(&options);
  hime_window_t window = hime_window_create(&options);
  ...
}

int main(int argc, const char** argv) {
  ...
  hime_lifetime_on_ready_connect(lifetime, OnReady, NULL);
  ...
}
```

After connecting the `ready` event, the program should enter the message loop
with the `Lifetime` API:

```c
int main(int argc, const char** argv) {
  ...
  int exit_code = hime_lifetime_run_main(lifetime);
  ...
  return exit_code;
}
```

### Multi-process environment

When using Chrohime with content module, the app will run under multi-process
mode, which means the `main` function will run for each process, i.e. the code
in `main` will be executed for multiple times in different processes.

So it is important to put GUI code in the callback of `ready` event instead of
the `main` function.

## A minimal app

Putting all together, this is what a minimal Chrohime app looks like:

```c
void OnReady(void* data) {
}

int main(int argc, const char** argv) {
  hime_state_create();
#if defined(_WIN32)
  hime_lifetime_t lifetime = hime_lifetime_create();
#else
  hime_lifetime_t lifetime = hime_lifetime_create(argc, argv);
#endif
  hime_lifetime_on_ready_connect(lifetime, OnReady, NULL);
  return hime_lifetime_run_main(lifetime);
}
```

To see how an app would look in real world, please see
[`gallery.c`](https://github.com/photoionization/chrohime/blob/main/gallery/gallery.c),
which is also shipped in the zip distributions of Chrohime.