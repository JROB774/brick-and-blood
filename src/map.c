INTERNAL void init_map ()
{
    // Nothing...
}

INTERNAL void quit_map ()
{
    // Nothing...
}

INTERNAL void render_map (int x, int y)
{
    int palette[2] = { PAL_TESTTILE0, PAL_TESTTILE1 };
    int index = 0;
    for (int iy=0; iy<MAP_TILE_H; ++iy)
    {
        for (int ix=0; ix<MAP_TILE_W; ++ix)
        {
            int sx = ((ix-iy)*(TILE_W/2)) + (SCREEN_W/2) - (TILE_W/2);
            int sy = ((ix+iy)*(TILE_H/2)) + (SCREEN_H/2) - ((MAP_TILE_H*TILE_H)/2);

            render_bitmap(sx,sy, ((index%2 == 0) ? palette[0] : palette[1]), &SPR_TILE);
            index++;
        }
        SWAP(palette[0],palette[1], int);
    }
}
