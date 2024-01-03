---
priority: 18
description: Why Chrohime was created and what it can be used for.
---

# Technical visions

The `ui/views` library is Chromium's internal UI toolkit for creating the
non-web parts of GUI, e.g. address bar, tabs, menus. While being a specialzied
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

A large part of desktop apps (including some of the most popular ones) are now
essentially web pages running on custom Chromuim browsers, via frameworks like Electron and CEF.

Chrohime is designed to be easily integrated into those Chromium-based apps,
which can then make use of native UI to implement performance sensitive
components, or achieve features impossible with web-only techs.

For Electron apps, Chrohime can be built as a component of Electron and have its
APIs exposed to native Node.js modules, which allows Electron apps to create
native UI. Interoperability with Electron's UI components `WebContentsView` and
`BrowserWindow` is also possible with a few glueing code.

For CEF apps, with a few bridging code it is possible to put the browser view
created by CEF into the view hierarchy managed by Chrohime, so developers can
let different libraries do what they are good at: manage the browser engine with
CEF, and create UI with Chrohime.

(Note: above integrations have not been implemented yet at the time of writing,
as the current focus is to ship an independent library first.)

### An addition, not a killer

Chrohime is more about being a nice addition to existing apps, rather than being
a xxx-killer. It does not try to replace any existing UI toolkit, instead it
means to be a complement when developers run into limitations of web techs or
want to do some extreme performance optimizations.

## Why C API

Chrohime is designed to be integrated into scripting languages, and C APIs make
language bindings easier to write, especially for langauges not implemeneted in
C/C++.

Another reason is C++ ABI is too hard to get right.

To link with a C++ shared library that uses STL in its APIs, developers have to
make sure that the executable uses the same C++ standard library with the same
flags, which is a big problem for Chromium-based projects because Chromium uses
a version of libc++ newer than most systems provide. Additionally, Chromium
links with libc++ statically, so it is difficult to understand what will happen
when passing C++ objects through DLL boundries.

## Use cases outside Chromium-based apps

While Chrohime is designed to be integrated into Chromium codebases, it is also
designed to be used as an independent GUI library, and you can find prebuilt
binaries in the [Releases](https://github.com/photoionization/chrohime/releases)
page.

The biggest downside of building apps with Chrohime is apparent: the binary size
is too large, way higher than many mainstream GUI libraries. So I wouldn't
recommend using this library if you aim to write apps that take minimal disk
spaces.

But in the meanwhile this is a C library thats aims to be used a shared module,
so binary size is currently not a priority. In future we will try to provide
flags to disable certain components to reduce binary size.

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
