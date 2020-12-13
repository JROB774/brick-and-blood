INTERNAL void InitGame ()
{
    InitEntities();
    InitTiles();
    InitMap();

    MapSpawnEntity("player", 0,0);
    for (int i=0; i<25; ++i)
    {
        MapSpawnEntity("chicken", RandomRange(0,19),RandomRange(0,14));
    }
}

INTERNAL void QuitGame ()
{
    QuitMap();
}

INTERNAL void UpdateGame ()
{
    UpdatePlayer();
    UpdateMap();
}

INTERNAL void RenderGame ()
{
    RenderMap();
}
