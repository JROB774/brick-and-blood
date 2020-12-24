#pragma once

GLOBAL struct MenuState
{
    bool leaving;
} gMenuState;

INTERNAL void  StartMenu ();
INTERNAL void UpdateMenu ();
INTERNAL void RenderMenu ();
