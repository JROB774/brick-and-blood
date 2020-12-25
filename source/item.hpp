#pragma once

// General-purpose item systems and functionality.

struct ItemIngredient
{
    std::string type;
    int amount;
};

struct ItemRecipe
{
    std::string equipment;
    std::vector<ItemIngredient> ingredients;
};

struct ItemBase
{
    std::string pretty_name;
    std::string category;
    SDL_Rect clip;
    Vec4 color;
    std::string place;
    float hunger;
    int stack;
    int damage;
    ItemRecipe recipe;
};

INTERNAL ItemBase& GetItem (std::string name);

INTERNAL void InitItems ();
