#pragma once

// General-purpose item systems and functionality.

struct ItemBase
{
    SDL_Rect clip;
    Vec4 color;
    std::string place;
};

INTERNAL ItemBase& GetItem (std::string name);

INTERNAL void InitItems ();
