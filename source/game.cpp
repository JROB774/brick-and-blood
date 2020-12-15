INTERNAL void InitGame ()
{
    InitEntities();
    InitTiles();
    InitMap();

    for (int i=0; i<150; ++i)
    {
        int x = RandomRange(0,WORLD_W_IN_TILES-1);
        int y = RandomRange(0,WORLD_H_IN_TILES-1);
        MapSpawnEntity("chicken",x,y);
    }
}

INTERNAL void QuitGame ()
{
    QuitMap();
}

INTERNAL void UpdateGame ()
{
    // Generate a whole new world map.
    #if defined(BUILD_DEBUG)
    if (IsKeyPressed(SDL_SCANCODE_F5)) InitMap();
    #endif

    UpdatePlayer();
    UpdateMap();
}

INTERNAL void RenderGame ()
{
    BeginCamera();
    RenderMap();
    EndCamera();
}
