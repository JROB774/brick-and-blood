#ifndef RENDERER_HPP
#define RENDERER_HPP

enum Flip
{
    FLIP_NONE,
    FLIP_HORZ,
    FLIP_VERT
};

INTERNAL void InitRenderer ();
INTERNAL void QuitRenderer ();

//
// Immediate mode drawing interface.
//

INTERNAL void ImmSetProjection (Mat4 projection_matrix);
INTERNAL void ImmSetView       (Mat4 view_matrix);

INTERNAL void ImmDrawPoint (float x,  float y,                      Vec4 color);
INTERNAL void ImmDrawLine  (float x1, float y1, float x2, float y2, Vec4 color);
INTERNAL void ImmDrawRect  (float x,  float y,  float w,  float h,  Vec4 color);
INTERNAL void ImmDrawFill  (float x,  float y,  float w,  float h,  Vec4 color);
INTERNAL void ImmDrawRect  (Rect rect,                              Vec4 color);
INTERNAL void ImmDrawFill  (Rect rect,                              Vec4 color);
INTERNAL void ImmDrawImage (std::string image_name, float x, float y, Vec2 center = { 0,0 }, float angle = 0.0f, Flip flip = FLIP_NONE, Vec4 color = { 1,1,1,1 }, const Rect* clip = NULL); // Angle in radians!
INTERNAL void ImmDrawText  (std::string font_name, std::string text, float x, float y, Vec4 color = { 1,1,1,1 });

#endif /* RENDERER_HPP */
