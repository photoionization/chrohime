// Copyright 2023 Microsoft, Inc
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROHIME_CHROHIME_EXPORT_H_
#define CHROHIME_CHROHIME_EXPORT_H_

#if defined(COMPONENT_BUILD)
#if defined(WIN32)

#if defined(CHROHIME_IMPLEMENTATION)
#define CHROHIME_EXPORT __declspec(dllexport)
#else
#define CHROHIME_EXPORT __declspec(dllimport)
#endif  // defined(CHROHIME_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(CHROHIME_IMPLEMENTATION)
#define CHROHIME_EXPORT __attribute__((visibility("default")))
#else
#define CHROHIME_EXPORT
#endif  // defined(CHROHIME_IMPLEMENTATION)
#endif

#else  // defined(COMPONENT_BUILD)
#define CHROHIME_EXPORT
#endif

#endif  // CHROHIME_CHROHIME_EXPORT_H_
