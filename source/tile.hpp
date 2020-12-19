#pragma once

// General-purpose tile systems and functionality.

GLOBAL constexpr float TILE_COLOR_SPEED =  4.0f;
GLOBAL constexpr float TILE_TURN_SPEED  = 15.0f;
GLOBAL constexpr float TILE_HIT_ANGLE   = 30.0f;

GLOBAL constexpr int TILE_INDESTRUCTIBLE = -9999;

struct Tile
{
    std::string type;

    int hits;

    bool solid;
    bool active;

    struct
    {
        SDL_Rect clip;

        struct
        {
            float current;
            float target;
        }  angle;
        struct
        {
            Vec4 current;
            Vec4 target;
        } color;
    } draw;
};

struct TileDrops
{
    std::string type;
    int min, max;
};

struct TileBase
{
    int hits;
    struct { int x,y; } image;
    Vec4 color;
    bool solid;
    std::vector<std::string> particle_hit;
    std::vector<std::string> particle_break;
    std::vector<std::string> sound_hit;
    std::vector<TileDrops> drops;
};

INTERNAL void InitTiles ();

//
// HELPERS
//

INTERNAL void DamageTile (int x, int y);
