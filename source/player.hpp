#ifndef PLAYER_HPP
#define PLAYER_HPP

struct Player
{
    Vec2 position;
};

INTERNAL void CreatePlayer (Player& player, float x, float y);
INTERNAL void DeletePlayer (Player& player);
INTERNAL void UpdatePlayer (Player& player);
INTERNAL void RenderPlayer (Player& player);

#endif // PLAYER_HPP
