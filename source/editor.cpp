GLOBAL constexpr float EDITOR_PALETTE_WIDTH    = 320.0f;
GLOBAL constexpr float EDITOR_PALETTE_BORDER   =   4.0f;
GLOBAL constexpr float EDITOR_ICON_SCALE       =   2.0f;
GLOBAL constexpr float EDITOR_ICON_PADDING     =   5.0f;
GLOBAL constexpr float EDITOR_ICON_HOVER_SCALE =   2.4f;
GLOBAL constexpr Vec4  EDITOR_ICON_HOVER_COLOR = { 1.00f,1.00f,1.00f,1.0f };
GLOBAL constexpr Vec4  EDITOR_COLOR0           = { 0.28f,0.18f,0.24f,1.0f };
GLOBAL constexpr Vec4  EDITOR_COLOR1           = { 0.81f,0.77f,0.72f,1.0f };

struct EditorIcon
{
    std::string type;

    // Drawing stuff.
    struct
    {
        SDL_Rect clip;

        struct
        {
            float current;
            float target;
        } scale;
        struct
        {
            Vec4 current;
            Vec4 target;
            Vec4 base;
        } color;
    } draw;
};

GLOBAL struct Editor
{
    std::vector<EditorIcon> tile_icons;
    std::vector<EditorIcon> entity_icons;

} gEditor;

INTERNAL void InitEditor ()
{
    // Cache information on all the tiles and entities for the ditor.

    // Tiles
    for (auto& [k,t]: gTiles)
    {
        EditorIcon icon;
        icon.type = k;
        icon.draw.clip = { t.image.x*TILE_W, t.image.y*TILE_H, TILE_W, TILE_H };
        icon.draw.scale.current = EDITOR_ICON_SCALE;
        icon.draw.scale.target = EDITOR_ICON_SCALE;
        icon.draw.color.current = t.color;
        icon.draw.color.target = t.color;
        icon.draw.color.base = t.color;
        gEditor.tile_icons.push_back(icon);
    }

    // Entities
    for (auto& [k,e]: gEntities)
    {
        EditorIcon icon;
        icon.type = k;
        icon.draw.clip = { e.image.x*TILE_W, e.image.y*TILE_H, TILE_W, TILE_H };
        icon.draw.scale.current = EDITOR_ICON_SCALE;
        icon.draw.scale.target = EDITOR_ICON_SCALE;
        icon.draw.color.current = e.color;
        icon.draw.color.target = e.color;
        icon.draw.color.base = e.color;
        gEditor.entity_icons.push_back(icon);
    }
}

INTERNAL void QuitEditor ()
{
    // Nothing...
}

INTERNAL void DoEditorPaletteIcons (float& cx, float& cy, float px, float py, std::vector<EditorIcon>& icons, std::string image)
{
    constexpr float ICON_W = TILE_W * EDITOR_ICON_SCALE;
    constexpr float ICON_H = TILE_H * EDITOR_ICON_SCALE;

    Vec2 mouse = GetMousePos();

    for (auto& t: icons)
    {
        if (cx+ICON_W > GetWindowWidth())
        {
            cx = EDITOR_ICON_PADDING;
            cy += ICON_H + EDITOR_ICON_PADDING;
        }

        float x = px+cx;
        float y = py+cy;

        // If the mouse is hovering over the editor icon then apply some feedback effects.
        // We do this before applying scale so the bounding box doesn't get messed with.
        Rect bounds = { x,y,ICON_W,ICON_H };
        if (PointAndRectCollision(mouse, bounds))
        {
            t.draw.color.current = EDITOR_ICON_HOVER_COLOR;
            t.draw.color.target  = EDITOR_ICON_HOVER_COLOR;
            t.draw.scale.target  = EDITOR_ICON_HOVER_SCALE;
        }
        else
        {
            t.draw.color.target = t.draw.color.base;
            t.draw.scale.target = EDITOR_ICON_SCALE;
        }

        // Apply lerping effects.
        t.draw.color.current = Lerp(t.draw.color.current, t.draw.color.target, gApplication.delta_time*15);
        t.draw.scale.current = Lerp(t.draw.scale.current, t.draw.scale.target, gApplication.delta_time*30);

        // Adjust the X and Y for the current scale of the icon so it remains centered.
        x -= (TILE_W * (t.draw.scale.current - EDITOR_ICON_SCALE)) / 2;
        y -= (TILE_H * (t.draw.scale.current - EDITOR_ICON_SCALE)) / 2;

        Vec2 scale = { t.draw.scale.current, t.draw.scale.current };
        DrawImage(image, x,y, scale, {0,0}, 0.0f, FLIP_NONE, t.draw.color.current, &t.draw.clip);
        cx += ICON_W + EDITOR_ICON_PADDING;
    }
}

INTERNAL void DoEditorPalette ()
{
    float ww = (float)GetWindowWidth();
    float wh = (float)GetWindowHeight();

    float px = ww - (EDITOR_PALETTE_WIDTH + EDITOR_PALETTE_BORDER);
    float py = EDITOR_PALETTE_BORDER;
    float pw = EDITOR_PALETTE_WIDTH;
    float ph = wh - (EDITOR_PALETTE_BORDER * 2);

    DrawFill(px,py,pw,ph, EDITOR_COLOR0);

    // List all of the placeable tiles and entities.
    float cx = EDITOR_ICON_PADDING, cy = EDITOR_ICON_PADDING;
    DoEditorIcons(cx,cy, px,py, gEditor.tile_icons, "tile");
    cx = EDITOR_ICON_PADDING, cy += (TILE_H*EDITOR_ICON_SCALE) + (EDITOR_ICON_PADDING*3);
    DoEditorIcons(cx,cy, px,py, gEditor.entity_icons, "entity");
}

INTERNAL void DoEditor ()
{
    DoEditorPalette();
}
