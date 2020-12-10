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

    // UpdateParticles();
    // UpdateCamera();
}

INTERNAL void RenderApplication (float dt)
{
    ClearWindow({ 1,0,0,1 });

    switch (gApplication.state)
    {
        case (APP_STATE_GAME): RenderGame(); break;
    }

    // DrawParticles();
}
