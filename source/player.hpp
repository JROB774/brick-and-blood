#pragma once

GLOBAL constexpr int HOTBAR_ITEM_EMPTY = -1;
GLOBAL constexpr int HOTBAR_SIZE = 9;

enum PlayerState
{
    PLAYER_STATE_PLAY,
    PLAYER_STATE_INVENTORY,
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
        std::vector<InventoryItem> items;
        int selected_item;

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
    } hotbar;

} gPlayer;

INTERNAL void PlayerPickUpItem (std::string name, int amount);

INTERNAL void   InitPlayer ();
INTERNAL void UpdatePlayer ();
INTERNAL void RenderPlayer ();
