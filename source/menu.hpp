#pragma once

GLOBAL struct MenuState
{
    bool leaving;
} gMenuState;

INTERNAL void   InitMenu ();
INTERNAL void   QuitMenu ();
INTERNAL void UpdateMenu ();
INTERNAL void RenderMenu ();
