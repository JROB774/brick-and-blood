#pragma once

enum MenuMode
{
    MENU_MODE_TITLE,
    MENU_MODE_MAIN,
    MENU_MODE_HELP,
    MENU_MODE_CREDITS
};

enum MenuOption
{
    MENU_OPTION_PLAY,
    MENU_OPTION_HELP,
    MENU_OPTION_CREDITS,
    MENU_OPTION_EXIT,
    MENU_OPTION_TOTAL
};

GLOBAL struct MenuState
{
    MenuMode mode = MENU_MODE_TITLE;
    int option = MENU_OPTION_PLAY;
    bool changing;
} gMenuState;

INTERNAL void  StartMenu ();
INTERNAL void UpdateMenu ();
INTERNAL void RenderMenu ();
