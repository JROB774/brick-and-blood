GLOBAL constexpr float EDITOR_PALETTE_WIDTH    = 320.0f;
GLOBAL constexpr float EDITOR_PALETTE_BORDER   =   4.0f;
GLOBAL constexpr float EDITOR_ICON_SCALE       =   2.0f;
GLOBAL constexpr float EDITOR_ICON_PADDING     =   5.0f;
GLOBAL constexpr float EDITOR_ICON_HOVER_SCALE =   2.4f;
GLOBAL constexpr float EDITOR_ICON_HIT_SCALE   =   3.7f;
GLOBAL constexpr float EDITOR_ICON_HIT_ANGLE   = -30.0f;
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
            float current;
            float target;
        } angle;
        struct
        {
            Vec4 current;
            Vec4 target;
            Vec4 base;
        } color;
    } draw;
};

struct EditorIconIndex
{
    std::string type; // "tile" or "entity"
    int index;
};

struct EditorChunk
{
    EditorIconIndex things[CHUNK_W][CHUNK_H];
};

GLOBAL struct Editor
{
    std::vector<EditorIcon> tile_icons;
    std::vector<EditorIcon> entity_icons;

    Rect palette_bounds;

    EditorIconIndex selected;

    EditorChunk chunk;

} gEditor;

INTERNAL void InitEditor ()
{
    // Cache information on all the tiles and entities for the ditor.
    for (auto& [k,t]: gTiles)
    {
        EditorIcon icon;
        icon.type = k;
        icon.draw.clip = { t.image.x*TILE_W, t.image.y*TILE_H, TILE_W, TILE_H };
        icon.draw.scale.current = EDITOR_ICON_SCALE;
        icon.draw.scale.target = EDITOR_ICON_SCALE;
        icon.draw.angle.current = 0.0f;
        icon.draw.angle.target = 0.0f;
        icon.draw.color.current = t.color;
        icon.draw.color.target = t.color;
        icon.draw.color.base = t.color;
        gEditor.tile_icons.push_back(icon);
    }
    for (auto& [k,e]: gEntities)
    {
        EditorIcon icon;
        icon.type = k;
        icon.draw.clip = { e.image.x*TILE_W, e.image.y*TILE_H, TILE_W, TILE_H };
        icon.draw.scale.current = EDITOR_ICON_SCALE;
        icon.draw.scale.target = EDITOR_ICON_SCALE;
        icon.draw.angle.current = 0.0f;
        icon.draw.angle.target = 0.0f;
        icon.draw.color.current = e.color;
        icon.draw.color.target = e.color;
        icon.draw.color.base = e.color;
        gEditor.entity_icons.push_back(icon);
    }

    // Set the currently selected item to be the first tile.
    gEditor.selected.type = "tile";
    gEditor.selected.index = 0;
}

INTERNAL void QuitEditor ()
{
    // Nothing...
}

INTERNAL void DoEditorPaletteIcons (float& cx, float& cy, std::vector<EditorIcon>& icons, std::string image)
{
    constexpr float ICON_W = TILE_W * EDITOR_ICON_SCALE;
    constexpr float ICON_H = TILE_H * EDITOR_ICON_SCALE;

    Vec2 mouse = GetMousePos();

    for (size_t i=0; i<icons.size(); ++i)
    {
        EditorIcon& t = icons.at(i);

        if (cx+ICON_W > GetWindowWidth())
        {
            cx = EDITOR_ICON_PADDING;
            cy += ICON_H + EDITOR_ICON_PADDING;
        }

        float x = gEditor.palette_bounds.x + cx;
        float y = gEditor.palette_bounds.y + cy;

        Rect bounds = { x,y,ICON_W,ICON_H };

        // If the mouse is hovering over the editor icon then apply some feedback effects.
        // We do this before applying scale so the bounding box doesn't get messed with.
        if (PointAndRectCollision(mouse, bounds))
        {
            if (t.draw.color.target != EDITOR_ICON_HOVER_COLOR) // Important for pressed effects to work properly!
            {
                t.draw.color.current = EDITOR_ICON_HOVER_COLOR;
            }
            t.draw.color.target = EDITOR_ICON_HOVER_COLOR;
            t.draw.scale.target = EDITOR_ICON_HOVER_SCALE;

            if (IsMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                t.draw.scale.current = EDITOR_ICON_HIT_SCALE;
                t.draw.angle.current = EDITOR_ICON_HIT_ANGLE;
            }
        }
        else
        {
            t.draw.color.target = t.draw.color.base;
            t.draw.scale.target = EDITOR_ICON_SCALE;
        }

        // Apply lerping effects.
        t.draw.color.current = Lerp(t.draw.color.current, t.draw.color.target, gApplication.delta_time*15);
        t.draw.scale.current = Lerp(t.draw.scale.current, t.draw.scale.target, gApplication.delta_time*30);
        t.draw.angle.current = Lerp(t.draw.angle.current, t.draw.angle.target, gApplication.delta_time*20);

        // Adjust the X and Y for the current scale of the icon so it remains centered.
        x -= (TILE_W * (t.draw.scale.current - EDITOR_ICON_SCALE)) / 2;
        y -= (TILE_H * (t.draw.scale.current - EDITOR_ICON_SCALE)) / 2;

        Vec2 scale = { t.draw.scale.current, t.draw.scale.current };
        Vec2 center = { (TILE_W*scale.x)/2, (TILE_H*scale.y)/2 };
        DrawImage(image, x,y, scale, center, t.draw.angle.current, FLIP_NONE, t.draw.color.current, &t.draw.clip);
        cx += ICON_W + EDITOR_ICON_PADDING;
    }
}

INTERNAL void DoEditorPalette ()
{
    float ww = (float)GetWindowWidth();
    float wh = (float)GetWindowHeight();

    gEditor.palette_bounds.x = ww - (EDITOR_PALETTE_WIDTH + EDITOR_PALETTE_BORDER);
    gEditor.palette_bounds.y = EDITOR_PALETTE_BORDER;
    gEditor.palette_bounds.w = EDITOR_PALETTE_WIDTH;
    gEditor.palette_bounds.h = wh - (EDITOR_PALETTE_BORDER * 2);

    DrawFill(gEditor.palette_bounds, EDITOR_COLOR0);

    // List all of the placeable tiles and entities.
    float cx = EDITOR_ICON_PADDING, cy = EDITOR_ICON_PADDING;
    DoEditorPaletteIcons(cx,cy, gEditor.tile_icons, "tile");
    cx = EDITOR_ICON_PADDING, cy += (TILE_H*EDITOR_ICON_SCALE) + (EDITOR_ICON_PADDING*3);
    DoEditorPaletteIcons(cx,cy, gEditor.entity_icons, "entity");
}

INTERNAL void DoEditorCanvas ()
{
    // Nothing...
}

INTERNAL void DoEditor ()
{
    DoEditorPalette();
    DoEditorCanvas();
}
