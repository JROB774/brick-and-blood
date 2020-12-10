#ifndef PLAYER_HPP
#define PLAYER_HPP

struct Player
{
    struct { int x,y; } tile;

    // Stuff for drawing.
    Vec2 draw_pos;
    float current_angle;
    float target_angle;
};

INTERNAL void CreatePlayer (Player& player, int tilex, int tiley);
INTERNAL void DeletePlayer (Player& player);
INTERNAL void UpdatePlayer (Player& player);
INTERNAL void RenderPlayer (Player& player);

#endif // PLAYER_HPP
