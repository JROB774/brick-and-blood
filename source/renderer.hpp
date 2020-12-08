#ifndef RENDERER_HPP
#define RENDERER_HPP

typedef SDL_RendererFlip Flip;

GLOBAL constexpr Flip FLIP_NONE = SDL_FLIP_NONE;
GLOBAL constexpr Flip FLIP_HORZ = SDL_FLIP_HORIZONTAL;
GLOBAL constexpr Flip FLIP_VERT = SDL_FLIP_VERTICAL;

INTERNAL void DrawPoint (float x,  float y,                      Color color);
INTERNAL void DrawLine  (float x1, float y1, float x2, float y2, Color color);
INTERNAL void DrawRect  (float x,  float y,  float w,  float h,  Color color);
INTERNAL void DrawFill  (float x,  float y,  float w,  float h,  Color color);
INTERNAL void DrawRect  (Rect rect,                              Color color);
INTERNAL void DrawFill  (Rect rect,                              Color color);

INTERNAL void DrawImage   (Image& image, float x, float y,                     Flip flip = FLIP_NONE, const SDL_Rect* clip = NULL);
INTERNAL void DrawImageEx (Image& image, float x, float y, float angle = 0.0f, Flip flip = FLIP_NONE, const SDL_Rect* clip = NULL); // Angle in radians!
INTERNAL void DrawText    (Font& font, std::string text, float x, float y, Color color = { 1,1,1,1 });

#endif /* RENDERER_HPP */
