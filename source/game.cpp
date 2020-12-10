INTERNAL void InitGame ()
{
    SpawnEntity(ENTITY_TYPE_PLAYER, 0,0);
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
