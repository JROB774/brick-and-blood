#ifndef GAME_HPP
#define GAME_HPP

GLOBAL struct GameState
{
    Player player;

} gGameState;

INTERNAL void   InitGame ();
INTERNAL void   QuitGame ();
INTERNAL void UpdateGame ();
INTERNAL void RenderGame ();

#endif // GAME_HPP
