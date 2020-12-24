#pragma once

GLOBAL constexpr size_t HOTBAR_ITEM_EMPTY = 0;
GLOBAL constexpr int HOTBAR_SIZE = 9;

GLOBAL constexpr float PLAYER_MAX_HEALTH = 100.0f;
GLOBAL constexpr float PLAYER_MAX_HUNGER = 100.0f;

GLOBAL constexpr float PLAYER_HUNGER_GREAT = 98.0f;
GLOBAL constexpr float PLAYER_HUNGER_HIGH = 90.0f;
GLOBAL constexpr float PLAYER_HUNGER_LOW = 30.0f;
GLOBAL constexpr float PLAYER_HUNGER_CRITICAL = 5.0f;

GLOBAL constexpr float PLAYER_HUNGER_DECREMENT = 0.1f;
GLOBAL constexpr float PLAYER_HEALTH_INCREMENT = 0.2f;
GLOBAL constexpr float PLAYER_HEALTH_DECREMENT = 0.1f;

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
    size_t id;
    std::string name;
    int amount;
};

GLOBAL struct Player
{
    PlayerState state;

    float health;
    float hunger;

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
        size_t items[HOTBAR_SIZE]; // IDs for the inventory item vector.
        int selected_item;
    } hotbar;

} gPlayer;

INTERNAL std::string PlayerGetEquippedItemName ();

INTERNAL void PlayerPickUpItem (std::string name, int amount);

INTERNAL void PlayerEatSelectedItem ();
INTERNAL void PlayerPlaceSelectedItem (int x, int y);

INTERNAL void   InitPlayer ();
INTERNAL void UpdatePlayer ();
INTERNAL void RenderPlayer ();
