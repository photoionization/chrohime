// TODO(zcbenz): We should make an independent C interface for skia.

typedef uint32_t sk_color_t;

static inline
sk_color_t
sk_color_create_argb(unsigned a, unsigned r, unsigned g, unsigned b) {
  return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}

static inline
sk_color_t sk_color_create_rgb(unsigned r, unsigned g, unsigned b) {
  return sk_color_create_argb(0xFF, r, g, b);
}
