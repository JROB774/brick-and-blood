#pragma once

// #define DEBUG_ENEMY_TEST

#if !defined(DEBUG_ENEMY_TEST)
GLOBAL constexpr int   NIGHT_END = 650; // 100 turns
GLOBAL constexpr int EVENING_END = 550; // 150 turns
GLOBAL constexpr int     DAY_END = 400; // 400 turns
#else
GLOBAL constexpr int   NIGHT_END =  40;
GLOBAL constexpr int EVENING_END =   2;
GLOBAL constexpr int     DAY_END =   1;
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
