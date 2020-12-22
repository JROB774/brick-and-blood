INTERNAL void InitGame ()
{
    InitMap();
    InitPlayer();

    PlayerPickUpItem("planks", 30);
}

INTERNAL void QuitGame ()
{
    QuitMap();
}

INTERNAL void UpdateGame ()
{
    // Generate a whole new world map.
    #if defined(BUILD_DEBUG)
    if (IsKeyPressed(SDL_SCANCODE_R)) InitMap();
    #endif

    UpdatePlayer();
    UpdateMap();
}

INTERNAL void RenderGame ()
{
    BeginCamera();
    RenderMap();
    EndCamera();
    RenderPlayer();
}
