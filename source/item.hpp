#pragma once

// General-purpose item systems and functionality.

struct ItemBase
{
    SDL_Rect clip;
    Vec4 color;
};

INTERNAL ItemBase& GetItem (std::string name);

INTERNAL void InitItems ();
