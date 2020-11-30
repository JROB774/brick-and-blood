#ifndef RENDER_H
#define RENDER_H

// The size of the "screen" we render to internally.
// This can be scaled up based on the window size.
#define SCREEN_W 320 // Width
#define SCREEN_H 240 // Height

#define GLYPH_W 8
#define GLYPH_H 8

typedef U32 ARGBColor;

// Some useful macros for converting separate components into a U32 ARGB color.
#define MAKE_RGBA(r,g,b,a) (CAST(ARGBColor,(((a   )<<24)|((r)<<16)|((g)<<8)|(b))))
#define MAKE_RGB( r,g,b)   (CAST(ARGBColor,(((0xFF)<<24)|((r)<<16)|((g)<<8)|(b))))

typedef struct Clip__ { int x, y, w, h; } Clip;

#define PAL_MODE_DEFAULT 0

// INTERFACE

INTERNAL bool      init_renderer     ();
INTERNAL void      quit_renderer     ();
INTERNAL void      render_clear      (ARGBColor color);
INTERNAL void      render_display    ();
INTERNAL void      render_bitmap     (int x,  int y,  int palette_index, const Clip* clip);
INTERNAL void      render_text       (int x,  int y,  int palette_index, const char* text, ...);
INTERNAL void      render_point      (int x,  int y,                         ARGBColor color);
INTERNAL void      render_line       (int x1, int y1, int x2, int y2,        ARGBColor color);
INTERNAL void      render_rect       (int x,  int y,  int w,  int h,         ARGBColor color);
INTERNAL void      render_fill       (int x,  int y,  int w,  int h,         ARGBColor color);
INTERNAL void      render_circle     (int x,  int y,  int r,  int thickness, ARGBColor color);
INTERNAL void      set_palette_mode  (int palette_mode);
INTERNAL SDL_Rect  get_viewport      ();
INTERNAL int       get_text_w        (const char* text, ...);
INTERNAL int       get_text_h        (const char* text, ...);
INTERNAL ARGBColor get_palette_color (int palette_index, int color_index);
INTERNAL void      shake_camera      (int x, int y, float duration);
INTERNAL void      update_camera     (float dt);
INTERNAL void      begin_camera      ();
INTERNAL void      end_camera        ();

// SPRITES

GLOBAL const Clip SPR_TILE = { 0, 0, 16, 8 };

// ANIMATIONS

// ...

// PALETTES

#define PAL_BACKGROUND  0
#define PAL_WHITE       1
#define PAL_BLACK       2
#define PAL_TEXT_SHADE  3
#define PAL_GRASS0      4
#define PAL_GRASS1      5
#define PAL_DIRT0       6
#define PAL_DIRT1       7
#define PAL_SAND0       8
#define PAL_SAND1       9
#define PAL_WATER0     10
#define PAL_WATER1     11
#define PAL_DEEPWATER0 12
#define PAL_DEEPWATER1 13

#endif /* RENDER_H */
