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

typedef void (*hime_free_callback)(void* data);
