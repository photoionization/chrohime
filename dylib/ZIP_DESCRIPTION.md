There are 2 types of binaries provided:

* `chrohime-*.zip` - Shared library without content module (i.e. `webview` API).
* `chrohime_with_content-*.zip` - Shared library with content module.

For macOS, there is no `chrohime-*.zip` for now because the `ui/views` library
does not yet support macOS without content module.

Also, on macOS if you download and extract the zip files from the browser, the
gatekeeper will prevent you from loading the shared libraries. It is recommended
to use `curl`/`wget` to download the files instead.
