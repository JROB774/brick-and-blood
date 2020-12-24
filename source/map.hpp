#pragma once

GLOBAL constexpr int TILE_W  =  16; // In pixels!
GLOBAL constexpr int TILE_H  =  16;
GLOBAL constexpr int CHUNK_W =  10; // In tiles!
GLOBAL constexpr int CHUNK_H =   9;
GLOBAL constexpr int WORLD_W =   8; // In chunks!
GLOBAL constexpr int WORLD_H =   8;

GLOBAL constexpr int WORLD_W_IN_TILES = WORLD_W*CHUNK_W;
GLOBAL constexpr int WORLD_H_IN_TILES = WORLD_H*CHUNK_H;

GLOBAL constexpr int MAX_ENTITIES = 128;

GLOBAL constexpr int MIN_ENTITY_SPAWN_RADIUS  =  35; // Area where entities can spawn.
GLOBAL constexpr int MAX_ENTITY_SPAWN_RADIUS  =  65;
GLOBAL constexpr int MAX_ENTITY_ACTIVE_RADIUS = 100; // Area where entities start to despawn.

//
// INTERFACE
//

INTERNAL void    MapSpawnTile (std::string type, int x, int y);
INTERNAL void    MapSpawnEntity (std::string type, int x, int y, std::string state_override = "");
INTERNAL void    MapSpawnParticles (std::string type, Rect region);
INTERNAL Tile*   MapGetTileAtPos (int x, int y);             // NULL if pos is out of bounds!
INTERNAL Entity* MapGetEntityAtPos (int x, int y);           // NULL if no entity found!
INTERNAL Entity* MapGetFirstEntityOfType (std::string type); // NULL if no entity found!

//
// SYSTEMS
//

struct Chunk
{
    std::vector<std::vector<Tile>> tiles;

    Chunk ()
    {
        tiles.resize(CHUNK_H);
        for (auto& row: tiles) row.resize(CHUNK_W);
    }
};

GLOBAL struct Map
{
    std::vector<Entity> entities;
    std::vector<Particle> particles;

    std::vector<std::vector<Chunk>> chunks;

} gMap;

INTERNAL void   InitMap ();
INTERNAL void   QuitMap ();
INTERNAL void UpdateMap ();
INTERNAL void RenderMap ();
