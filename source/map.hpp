#pragma once

GLOBAL constexpr int TILE_W  = 16; // In pixels!
GLOBAL constexpr int TILE_H  = 16;
GLOBAL constexpr int CHUNK_W = 10; // In tiles!
GLOBAL constexpr int CHUNK_H =  9;
GLOBAL constexpr int WORLD_W = 20; // In chunks!
GLOBAL constexpr int WORLD_H = 20;

GLOBAL constexpr int WORLD_W_IN_TILES = WORLD_W*CHUNK_W;
GLOBAL constexpr int WORLD_H_IN_TILES = WORLD_H*CHUNK_H;

//
// INTERFACE
//

INTERNAL void    MapPlaceTile (std::string type, int x, int y);
INTERNAL void    MapSpawnEntity (std::string type, int x, int y);
INTERNAL Tile*   MapGetTileAtPos (int x, int y);             // NULL if pos is out of bounds!
INTERNAL Entity* MapGetEntityAtPos (int x, int y);           // NULL if no entity found!
INTERNAL Entity* MapGetFirstEntityOfType (std::string type); // NULL if no entity found!

//
// SYSTEMS
//

struct Chunk
{
    Tile tiles[CHUNK_H][CHUNK_W];
};

GLOBAL struct Map
{
    std::vector<Entity> entities;
    std::vector<Particle> particles;

    Chunk chunks[WORLD_H][WORLD_W];

} gMap;

INTERNAL void   InitMap ();
INTERNAL void   QuitMap ();
INTERNAL void UpdateMap ();
INTERNAL void RenderMap ();
