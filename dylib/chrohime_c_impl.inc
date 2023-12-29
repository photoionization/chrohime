#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// On macOS uchar.h does not exist and we must manually define char16_t.
#if !defined(__cplusplus)
#ifdef __APPLE__
typedef uint16_t char16_t;
#else
#include <uchar.h>
#endif
#endif  // !defined(__cplusplus)

#if defined(WIN32)
#if defined(CHROHIME_C_IMPLEMENTATION)
#define CHROHIME_C_EXPORT __declspec(dllexport)
#else
#define CHROHIME_C_EXPORT __declspec(dllimport)
#endif
#else  // defined(WIN32)
#if defined(CHROHIME_C_IMPLEMENTATION)
#define CHROHIME_C_EXPORT __attribute__((visibility("default")))
#else
#define CHROHIME_C_EXPORT
#endif
#endif  // !defined(WIN32)

// Color type.
typedef uint32_t hime_color_t;

// Helpers for creating colors.
static inline
hime_color_t
hime_color_create_argb(unsigned a, unsigned r, unsigned g, unsigned b) {
  return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

static inline
hime_color_t hime_color_create_rgb(unsigned r, unsigned g, unsigned b) {
  return hime_color_create_argb(0xFF, r, g, b);
}

// The function used for releasing the data passed in event handlers.
typedef void (*hime_free_callback)(void* data);
