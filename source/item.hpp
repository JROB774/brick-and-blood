#pragma once

// General-purpose item systems and functionality.

struct ItemIngredient
{
    std::string type;
    int amount;
};

struct ItemBase
{
    SDL_Rect clip;
    Vec4 color;
    std::string place;
    std::vector<ItemIngredient> recipe;
};

INTERNAL ItemBase& GetItem (std::string name);

INTERNAL void InitItems ();
