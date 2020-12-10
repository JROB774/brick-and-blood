INTERNAL void InitApplication ()
{
    gApplication.state = APP_STATE_GAME;
    InitParticleSystem();
    InitGame();
}

INTERNAL void QuitApplication ()
{
    QuitGame();
    QuitParticleSystem();
}

INTERNAL void UpdateApplication (float dt)
{
    gApplication.delta_time = dt;
    gApplication.total_time += dt;
    gApplication.frame++;

    switch (gApplication.state)
    {
        case (APP_STATE_GAME): UpdateGame(); break;
    }

    UpdateParticles();
    UpdateCamera();
}

INTERNAL void RenderApplication (float dt)
{
    constexpr Rect SCREEN = { 0,0,WINDOW_SCREEN_W,WINDOW_SCREEN_H };
    DrawFill(SCREEN, APP_SCREEN_CLEAR_COLOR);

    switch (gApplication.state)
    {
        case (APP_STATE_GAME): RenderGame(); break;
    }

    DrawParticles();
}
