INTERNAL void InitGame ()
{
    CreatePlayer(gGameState.player, 0,0);
}

INTERNAL void QuitGame ()
{
    DeletePlayer(gGameState.player);
}

INTERNAL void UpdateGame ()
{
    UpdatePlayer(gGameState.player);
}

INTERNAL void RenderGame ()
{
    RenderPlayer(gGameState.player);
}
