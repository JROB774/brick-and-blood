INTERNAL void InitGame ()
{
    InitEntities();
    InitTiles();
    InitMap();
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
