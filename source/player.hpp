#pragma once

GLOBAL constexpr int HOTBAR_ITEM_EMPTY = -1;
GLOBAL constexpr int HOTBAR_SIZE = 9;

enum PlayerState
{
    PLAYER_STATE_PLAY,
    PLAYER_STATE_INVENTORY,
};

enum InventoryState
{
    INVENTORY_STATE_ITEMS,
    INVENTORY_STATE_CRAFTING,
};

struct InventoryItem
{
    std::string name;
    int amount;
};

GLOBAL struct Player
{
    PlayerState state;

    float input_timer;
    bool update;

    struct
    {
        InventoryState state;

        std::vector<InventoryItem> items;
        int selected_item;

        std::vector<std::string> recipes;
        int selected_recipe;

        struct
        {
            Vec2 current;
            Vec2 target;
        } scale;
        struct
        {
            Rect current;
            Rect target;
        } bounds;

    } inventory;

    struct
    {
        int items[HOTBAR_SIZE]; // Points into the inventory items vector.
        int selected_item;
    } hotbar;

} gPlayer;

INTERNAL std::string PlayerGetEquippedItemName ();

INTERNAL void PlayerPickUpItem (std::string name, int amount);
INTERNAL void PlayerPlaceSelectedItem (int x, int y);

INTERNAL void   InitPlayer ();
INTERNAL void UpdatePlayer ();
INTERNAL void RenderPlayer ();
