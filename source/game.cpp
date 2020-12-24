INTERNAL void InitGame ()
{
    InitLighting();
    InitMap();
    InitPlayer();

    EnableLighting(true);
    SetLightingIntensity(0);
}

INTERNAL void QuitGame ()
{
    QuitMap();
    QuitLighting();
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
    BeginLighting();
    BeginCamera();
    RenderMap();
    EndCamera();
    EndLighting();
    RenderPlayer();
}
