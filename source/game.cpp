INTERNAL void InitGame ()
{
    InitEntities();
    SpawnEntity("player", 0,0);
}

INTERNAL void QuitGame ()
{
    // Nothing...
}

INTERNAL void UpdateGame ()
{
    UpdateEntities();
}

INTERNAL void RenderGame ()
{
    RenderEntities();
}
