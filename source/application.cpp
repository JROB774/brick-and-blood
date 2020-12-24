GLOBAL constexpr Vec4 APPLICATION_CLEAR = { 0.28f,0.18f,0.24f,1.0f };

INTERNAL void InitApplication ()
{
    SDL_ShowCursor(SDL_DISABLE);

    gApplication.debug = false;
    gApplication.editor = false;

    InitParticles();
    InitEntities();
    InitTiles();
    InitItems();
    InitFade();
    InitLighting();
    InitEditor();

    StartMenu();

    FadeFrom(SDLColorToColor({ 38,13,28,255 }), 2);
}

INTERNAL void QuitApplication ()
{
    QuitLighting();
    QuitEditor();
}

// Debug commands that can be called using the F1-12 keys in a debug build.
#if defined(BUILD_DEBUG)
INTERNAL void ToggleEditor ()
{
    gApplication.editor = !gApplication.editor;

    if (gApplication.editor) SDL_ShowCursor(SDL_ENABLE);
    else SDL_ShowCursor(SDL_DISABLE);

    if (!gApplication.editor)
    {
        EditorSaveZoneChunks();
    }
}
INTERNAL void ToggleDebug ()
{
    gApplication.debug = !gApplication.debug;
}
INTERNAL void ReloadResources ()
{
    InitParticles();
    InitEntities();
    InitTiles();
}
#endif // BUILD_DEBUG

INTERNAL void UpdateApplication (float dt)
{
    gApplication.delta_time = dt;
    gApplication.total_time += dt;
    gApplication.frame++;

    // We do not want these debug keys enabled in release builds!
    #if defined(BUILD_DEBUG)
    if (IsKeyPressed(SDL_SCANCODE_F1)) ToggleEditor();
    if (IsKeyPressed(SDL_SCANCODE_F2)) ToggleDebug();
    if (IsKeyPressed(SDL_SCANCODE_F5)) ReloadResources();
    #endif // BUILD_DEBUG

    if (!gApplication.editor)
    {
        switch (gApplication.state)
        {
            case (APP_STATE_MENU): UpdateMenu(); break;
            case (APP_STATE_GAME): UpdateGame(); break;
            case (APP_STATE_LOSE): UpdateLose(); break;
        }

        UpdateCamera();
    }
}

INTERNAL void RenderApplication (float dt)
{
    if (!gApplication.editor)
    {
        SetViewport();

        constexpr Rect SCREEN = { 0,0,WINDOW_SCREEN_W,WINDOW_SCREEN_H };
        DrawFill(SCREEN, APPLICATION_CLEAR);

        switch (gApplication.state)
        {
            case (APP_STATE_MENU): RenderMenu(); break;
            case (APP_STATE_GAME): RenderGame(); break;
            case (APP_STATE_LOSE): RenderLose(); break;
        }

        RenderFade();
        UnsetViewport();
    }
    else
    {
        // Updates and renders the editor.
        DoEditor();
    }
}
