# Technical visions

The `ui/views` library is Chromium's internal UI toolkit for creating the
non-web parts of GUI, e.g. addressbar, tabs, menus. While being a specialzied
UI toolkit for browser GUI initially, it has evolved into a library for general
GUI, partly thanks to the needs of Chrome OS.

It has every feature that a desktop app would need: accessibility, animation,
event handling, text rendering, advanced 2D rendering, GPU acceleration, i18n,
image, vector icon, drag and drop, etc. Which is probably not surprising since
Chromium needs all those things to render web pages, but from a GUI library
writer's perspective, it is an engineering miracle that Chromium has created a
cross-platform native GUI library reusing all those components.

The Chrohime project (which means 黒姫 / black princess, a beautiful village in
Nagano, Japan) is an ongoing effort to provide an independent GUI library with
C APIs based on the `ui/views` library.

## Why another GUI library

A large part of desktop apps are now essentially web pages running on custom
Chromuim browsers, via frameworks like Electron and CEF, for them having
the ability to create native UI would be a great addition.

Chrohime is designed to be integrated into those apps. (Note: none of the
following things have been implemented at the time of writing, though you can
trust me that there is no technical difficulties.)

For Electron, it is possible to build Chrohime as part of Electron and expose
its C APIs to native Node.js modules, which can then be used to create
JavaScript APIs that allow Electron apps to create native UI. And it is possible
to integrate the WebContentsView created by Electron into the view hierarchy of
Chrohime, so Electron apps can create mixed UI with web pages and native UI like
how the Chrome browser itself is implemented.

For CEF, with a few bridging code it is possible to put the browser view created
by CEF into the view hierarchy managed by Chrohime, so you can let different
libraries do what they are good at: manage the browser engine with CEF, and
create UI with Chrohime.

### An addition, not a killer

As you can see, Chrohime is more about being a nice addition to existing apps,
rather than being a xxx-killer. If you are totally happy with Electron, you can
surely just ignore this library, but if you run into something hard to achieve
with web techs, Chrohime will be there to help.

## Why C API

Chrohime provides C APIs instead of C++ APIs mostly because C++ ABI is too hard
to get right.

To link with a C++ shared library, developers have to be sure that the
executable uses the same C++ standard library with the same flags, which is a
big problem for Chromium-based projects because Chromium uses a version of
libc++ newer than most systems provide. Additionally, Chromium links with libc++
statically, so it is difficult to understand what will happen when passing C++
objects through DLL boundries.

On the other hand, it is much easier integrating a C library for scripting
languages, especially for those not written in C/C++.

## Use cases outside Chromium-based apps

While Chrohime is designed to be integrated into Chromium codebases, it is also
designed to be used as an independent GUI library, and you can find prebuilt
binaries in the [Releases](https://github.com/photoionization/chrohime/releases)
page.

The biggest downside of building apps with Chrohime is apparent: the binary size
is too large, way higher than many mainstream GUI libraries. So I wouldn't
recommend using this library if you aim to write apps that take minimal disk
spaces.

But in the meanwhile this is a C library thats aims to be friendly with language
bindings, so binary size is not a priority since the binary is supposed to be
shared as modules.

### React Native on desktop

One perfect use case for Chrohime outside Chromium codebases is Rect Native.

There are already a few implementaions of React Native backends for desktop, but
the most popular implementations are using native platform APIs, i.e. each
platform has its own independent implementations. With Chrohime it is possible
to write a Rect Native backend with one single implementation for all platforms.

### Alternative for WebView2

Microsoft provides a shared Chromium-based browser runtime with COM APIs, named
[WebView2](https://developer.microsoft.com/en-us/microsoft-edge/webview2), which
is quite handy when you want to create a desktop app for your website without
shipping a Chromium engine.

We can do the same for Chrohime, by providing a global Chrohime shared library
in operating systems, developers can easily get the same benefits with WebView2.
And the better thing is, since Chrohime is a C library, developers will have
same experiences across all platforms using any language.