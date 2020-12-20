#pragma once

// General-purpose item systems and functionality.

struct ItemBase
{
    SDL_Rect clip;
    Vec4 color;
};

INTERNAL void InitItems ();
