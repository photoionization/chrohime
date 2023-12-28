# chrohime 👸🏾

Chrohime is a C library based on Chromium's `ui/views` library, for building
cross-platform GUI on desktop.

Goals:

* A standalone C library to be used by any program.
* Easy integration with scripting languages.
* Ability to be shipped inside Electron.
* Interoperability with Electron APIs.
* Interoperability with CEF APIs.
* Optional builds with Chromium content module.
* Simple implementation of React Native.
* APIs for system webview.

## Documentations

API references are not ready yet, but most APIs are commented in the
`chrohime.h` header, which can be found in the zip distributions in the
[Releases](https://github.com/photoionization/chrohime/releases) page.

* [Screenshots📷](https://github.com/photoionization/chrohime/issues/1)
* [Technical visions](https://github.com/photoionization/chrohime/blob/main/docs/tutorial/technical-visions.md)
* [App architecture](https://github.com/photoionization/chrohime/blob/main/docs/tutorial/app-architecture.md)
* [Object types](https://github.com/photoionization/chrohime/blob/main/docs/tutorial/object-types.md)

### Build

Download Chromium source code:

```
./scripts/bootstrap.py
```

Build Chrohime:

```
./scripts/gn_gen.py
./scripts/build.py --config Release
```

### Source code structure

* `/api` - Wrappers of the `ui/views` classes.
* `/content` - Implementations of content delegates.
* `/chromium` - Downloaded Chromium source code.
* `/docs` - Descriptions of APIs used for code and documentation generation.
* `/dylib` - C APIs of the `/api` wrappers.
* `/gallery` - Sample app shipped in the zip distributions.
* `/test` - Tests of the C APIs and C++ wrappers.
* `/scripts` - Scripts for building Chrohime.

## License

Same with Chromium.
