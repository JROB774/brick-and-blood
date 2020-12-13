#pragma once

// General-purpose tile systems and functionality.

struct Tile
{
    std::string type;

    int hits;

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

INTERNAL void InitTiles ();
