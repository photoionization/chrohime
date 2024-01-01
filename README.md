# chrohime 👸🏾

Chrohime is a C library based on Chromium's `ui/views` library, for building
native cross-platform GUI on desktop.

You can Learn more about the tech by reading
[technical visions](https://chrohime.com/docs/latest/c/guide/technical-visions.html).

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

* [Screenshots📷](https://github.com/photoionization/chrohime/issues/1)
* [API references](https://chrohime.com/docs/latest/c)

## Build

Download Chromium source code:

```
./scripts/bootstrap.py
```

Build Chrohime:

```
./scripts/gn_gen.py
./scripts/build.py --config Release
```

## Source code structure

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
