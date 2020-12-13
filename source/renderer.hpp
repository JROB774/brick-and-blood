#pragma once

typedef SDL_RendererFlip Flip;

GLOBAL constexpr Flip FLIP_NONE = SDL_FLIP_NONE;
GLOBAL constexpr Flip FLIP_HORZ = SDL_FLIP_HORIZONTAL;
GLOBAL constexpr Flip FLIP_VERT = SDL_FLIP_VERTICAL;

INTERNAL void DrawPoint (float x,  float y,                      Vec4 color);
INTERNAL void DrawLine  (float x1, float y1, float x2, float y2, Vec4 color);
INTERNAL void DrawRect  (float x,  float y,  float w,  float h,  Vec4 color);
INTERNAL void DrawFill  (float x,  float y,  float w,  float h,  Vec4 color);
INTERNAL void DrawRect  (Rect rect,                              Vec4 color);
INTERNAL void DrawFill  (Rect rect,                              Vec4 color);

INTERNAL void DrawImage (std::string image_name, float x, float y, Vec2 scale = {1,1}, Vec2 center = {0,0}, float angle = 0.0f, Flip flip = FLIP_NONE, Vec4 color = { 1,1,1,1 }, const SDL_Rect* clip = NULL); // Angle in degrees!
INTERNAL void DrawText  (std::string font_name, std::string text, float x, float y, Vec4 color = { 1,1,1,1 });
