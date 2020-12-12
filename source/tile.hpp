#pragma once

// General-purpose tile systems and functionality.

struct Tile
{
    std::string base_type;

    struct { int x,y; } pos;
}
