#ifndef MAP_H
#define MAP_H

#define MAP_TILE_W 128
#define MAP_TILE_H 128

#define TILE_W 16
#define TILE_H 8

typedef struct Tile__
{
    float height;

} Tile;

typedef struct Map__
{
    Tile tile[MAP_TILE_W*MAP_TILE_H];

} Map;

GLOBAL Map gMap;

INTERNAL void   init_map ();
INTERNAL void   quit_map ();
INTERNAL void render_map (int x, int y);

#endif /* MAP_H */
