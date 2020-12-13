INTERNAL void InitApplication ()
{
    gApplication.state = APP_STATE_GAME;
    gApplication.editor = false;

    InitParticleSystem();
    InitGame();
    InitEditor();
}

INTERNAL void QuitApplication ()
{
    QuitEditor();
    QuitGame();
    QuitParticleSystem();
}

INTERNAL void UpdateApplication (float dt)
{
    gApplication.delta_time = dt;
    gApplication.total_time += dt;
    gApplication.frame++;

    if (IsKeyPressed(SDL_SCANCODE_F1))
    {
        gApplication.editor = !gApplication.editor;
        if (IsFullscreen())
        {
            if (gApplication.editor) SDL_ShowCursor(SDL_ENABLE);
            else SDL_ShowCursor(SDL_DISABLE);
        }
    }

    if (!gApplication.editor)
    {
        switch (gApplication.state)
        {
            case (APP_STATE_GAME): UpdateGame(); break;
        }

        UpdateParticles();
        UpdateCamera();
    }
}

INTERNAL void RenderApplication (float dt)
{
    if (!gApplication.editor)
    {
        SetViewport();

        constexpr Rect SCREEN = { 0,0,WINDOW_SCREEN_W,WINDOW_SCREEN_H };
        DrawFill(SCREEN, { 0.28f,0.18f,0.24f,1.0f });

        switch (gApplication.state)
        {
            case (APP_STATE_GAME): RenderGame(); break;
        }

        DrawParticles();

        UnsetViewport();
    }
    else
    {
        // Updates and renders the editor.
        DoEditor();
    }
}
