INTERNAL void InitGame ()
{
    InitEntities();
    InitTiles();
    InitMap();
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
    RenderMap();
}
