#pragma once

#if !defined(DEBUG_ENEMY_TEST)
GLOBAL constexpr int   NIGHT_END = 650; // 100 turns
GLOBAL constexpr int EVENING_END = 550; // 150 turns
GLOBAL constexpr int     DAY_END = 400; // 400 turns
#else
GLOBAL constexpr int   NIGHT_END = 101;
GLOBAL constexpr int EVENING_END =  51;
GLOBAL constexpr int     DAY_END =  50;
#endif // DEBUG_ENEMY_TEST

GLOBAL struct GameState
{
    int turns;
} gGameState;

INTERNAL bool IsDay   ();
INTERNAL bool IsNight ();
INTERNAL int  GetDays ();

INTERNAL void  StartGame ();
INTERNAL void UpdateGame ();
INTERNAL void RenderGame ();
