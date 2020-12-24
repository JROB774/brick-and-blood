#pragma once

GLOBAL struct LoseState
{
    bool leaving;
    float cooldown;
} gLoseState;

INTERNAL void  StartLose ();
INTERNAL void UpdateLose ();
INTERNAL void RenderLose ();
