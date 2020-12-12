GLOBAL constexpr float EDITOR_PALETTE_WIDTH = 320.0f;
GLOBAL constexpr float EDITOR_ICON_SCALE = 1.0f;

GLOBAL const Vec4 EDITOR_COLOR0 = { 0.28f,0.18f,0.24f,1.0f };
GLOBAL const Vec4 EDITOR_COLOR1 = { 0.81f,0.77f,0.72f,1.0f };

INTERNAL void InitEditor ()
{
    // Nothing...
}

INTERNAL void QuitEditor ()
{
    // Nothing...
}

INTERNAL void DoEditor ()
{
    float ww = (float)GetWindowWidth();
    float wh = (float)GetWindowHeight();

    float px = ww - EDITOR_PALETTE_WIDTH;
    float py = 0;
    float pw = EDITOR_PALETTE_WIDTH;
    float ph = wh;

    DrawFill(px,py,pw,ph, EDITOR_COLOR0);

    // List all of the tiles.

    // List all of the entities.
}
