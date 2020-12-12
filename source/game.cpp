INTERNAL void InitGame ()
{
    InitEntities();

    SpawnEntity("player", 0,6);
    SpawnEntity("goblin", 4,5);
}

INTERNAL void QuitGame ()
{
    // Nothing...
}

INTERNAL void UpdateGame ()
{
    UpdatePlayer();
    if (gPlayer.update)
    {
        UpdateEntities();
    }
}

INTERNAL void RenderGame ()
{
    RenderEntities();
}
