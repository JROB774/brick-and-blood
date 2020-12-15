#pragma once

GLOBAL constexpr int TILE_W = 16;
GLOBAL constexpr int TILE_H = 16;

GLOBAL constexpr int CHUNK_W = WINDOW_SCREEN_W / TILE_W;
GLOBAL constexpr int CHUNK_H = WINDOW_SCREEN_H / TILE_H;

//
// INTERFACE
//

INTERNAL void    MapSpawnEntity    (std::string type, int x, int y);
INTERNAL Entity* MapGetEntityAtPos (int x, int y); // NULL if no entity found!

//
// SYSTEMS
//

struct Chunk
{
    // Nothing...
};

GLOBAL struct Map
{
    std::vector<Entity> entities;

} gMap;

INTERNAL void   InitMap ();
INTERNAL void   QuitMap ();
INTERNAL void UpdateMap ();
INTERNAL void RenderMap ();
