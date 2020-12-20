#pragma once

enum PlayerState
{
    PLAYER_STATE_PLAY,
    PLAYER_STATE_INVENTORY,
};

GLOBAL struct Player
{
    PlayerState state;

    float input_timer;
    bool update;

    struct
    {
        // The inventory is a lookup of different resources and the quanities of those resources.
        // For example searching up "stone" in the inventory would return the amount of stone.
        std::map<std::string,int> items;

        struct
        {
            Rect current;
            Rect target;
        } bounds;

    } inventory;

} gPlayer;

INTERNAL void PlayerPickUpItem (std::string name, int amount);

INTERNAL void   InitPlayer ();
INTERNAL void UpdatePlayer ();
INTERNAL void RenderPlayer ();
