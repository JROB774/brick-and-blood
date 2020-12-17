#pragma once

GLOBAL struct Player
{
    float input_timer;
    bool update;

    // The inventory is a lookup of different resources and the quanities of those resources.
    // For example searching up "stone" in the inventory would return the amount of stone.
    std::map<std::string,int> inventory;

} gPlayer;

INTERNAL void UpdatePlayer ();
