# chrohime 👸🏾

Chrohime is a C library based on Chromium's `ui/views` library, for building
cross-platform GUI on desktop.

This project is an ongoing experiment and may not ship in the end.

Goals:

* A standalone C library to be used by any program.
* Easy integration with scripting languages.
* APIs for system webview.
* Optional builds with Chromium content APIs.
* Ability to be shipped inside Electron.
* Interoperability with Electron APIs.
* Interoperability with CEF APIs.
* Simple implementation of React Native.

## Build

Download Chromium source code:

```
./scripts/bootstrap.py
```

Build Chrohime:

```
./build_chromium/gn_gen.py
./scripts/build.py
```

## Source code structure

* `/api` - Wrappers of the `ui/views` classes.
* `/c` - C APIs of the `/api` wrappers.
* `/docs` - Descriptions of APIs used for code generation.
* `/test` - Tests of the C APIs and C++ wrappers.
* `/build_chromium` - Tools for buillding Chromium.
* `/scripts` - Scripts for building Chrohime.

## License

Same with Chromium.
