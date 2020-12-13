GLOBAL constexpr float EDITOR_PALETTE_WIDTH            = 320.0f;
GLOBAL constexpr float EDITOR_PALETTE_BORDER           =   4.0f;
GLOBAL constexpr float EDITOR_PALETTE_ICON_SCALE       =   2.0f;
GLOBAL constexpr float EDITOR_PALETTE_ICON_PADDING     =   5.0f;
GLOBAL constexpr float EDITOR_PALETTE_ICON_HOVER_SCALE =   2.4f;
GLOBAL constexpr float EDITOR_PALETTE_ICON_HIT_SCALE   =   3.0f;
GLOBAL constexpr float EDITOR_PALETTE_ICON_HIT_ANGLE   = -30.0f;
GLOBAL constexpr float EDITOR_CURSOR_ALPHA             =   0.5f;
GLOBAL constexpr Vec4  EDITOR_COLOR0                   = { 0.28f,0.18f,0.24f,1.0f };
GLOBAL constexpr Vec4  EDITOR_COLOR1                   = { 0.81f,0.77f,0.72f,1.0f };
GLOBAL constexpr Vec4  EDITOR_COLOR2                   = { 1.00f,1.00f,1.00f,1.0f };

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
    size_t index;
};

struct EditorThing
{
    EditorIconIndex icon_index;

    bool active;

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

struct EditorChunk
{
    EditorThing things[CHUNK_H][CHUNK_W];
};

GLOBAL struct Editor
{
    std::vector<EditorIcon> tile_icons;
    std::vector<EditorIcon> entity_icons;

    struct
    {
        Rect bounds;
    } palette;
    struct
    {
        Rect space;
        Rect bounds;
        float scale;
    } canvas;

    struct
    {
        struct { int x,y; } pos;

        struct
        {
            Vec2 pos;
            struct
            {
                Vec4 current;
                Vec4 target;
            } color;
        } draw;

        EditorIconIndex selected;
    } cursor;

    EditorChunk chunk;

} gEditor;

INTERNAL EditorIcon* GetSelectedEditorIcon ()
{
    EditorIcon* icon = NULL;
    if (gEditor.cursor.selected.type == "tile")
    {
        if (!gEditor.tile_icons.empty())
        {
            icon = &gEditor.tile_icons.at(gEditor.cursor.selected.index);
        }
    }
    else if (gEditor.cursor.selected.type == "entity")
    {
        if (!gEditor.entity_icons.empty())
        {
            icon = &gEditor.entity_icons.at(gEditor.cursor.selected.index);
        }
    }
    return icon;
}

INTERNAL void InitEditor ()
{
    // Cache information on all the tiles and entities for the ditor.
    for (auto& [k,t]: gTiles)
    {
        EditorIcon icon;
        icon.type = k;
        icon.draw.clip = { t.image.x*TILE_W, t.image.y*TILE_H, TILE_W, TILE_H };
        icon.draw.scale.current = EDITOR_PALETTE_ICON_SCALE;
        icon.draw.scale.target = EDITOR_PALETTE_ICON_SCALE;
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
        icon.draw.scale.current = EDITOR_PALETTE_ICON_SCALE;
        icon.draw.scale.target = EDITOR_PALETTE_ICON_SCALE;
        icon.draw.angle.current = 0.0f;
        icon.draw.angle.target = 0.0f;
        icon.draw.color.current = e.color;
        icon.draw.color.target = e.color;
        icon.draw.color.base = e.color;
        gEditor.entity_icons.push_back(icon);
    }

    // Set the currently selected item to be the first tile.
    gEditor.cursor.selected.type = "tile";
    gEditor.cursor.selected.index = 0;

    gEditor.cursor.pos.x = 0;
    gEditor.cursor.pos.y = 0;
    gEditor.cursor.draw.pos.x = 0.0f;
    gEditor.cursor.draw.pos.y = 0.0f;
    gEditor.cursor.draw.color.current = GetSelectedEditorIcon()->draw.color.base;
    gEditor.cursor.draw.color.target = GetSelectedEditorIcon()->draw.color.base;
    gEditor.cursor.draw.color.current.a = EDITOR_CURSOR_ALPHA;
    gEditor.cursor.draw.color.target.a = EDITOR_CURSOR_ALPHA;
}

INTERNAL void QuitEditor ()
{
    // Nothing...
}

INTERNAL void DoEditorPaletteIcons (float& cx, float& cy, std::vector<EditorIcon>& icons, std::string image)
{
    constexpr float ICON_W = TILE_W * EDITOR_PALETTE_ICON_SCALE;
    constexpr float ICON_H = TILE_H * EDITOR_PALETTE_ICON_SCALE;

    Vec2 mouse = GetMousePos();

    for (size_t i=0; i<icons.size(); ++i)
    {
        EditorIcon& t = icons.at(i);

        if (cx+ICON_W > GetWindowWidth())
        {
            cx = EDITOR_PALETTE_ICON_PADDING;
            cy += ICON_H + EDITOR_PALETTE_ICON_PADDING;
        }

        float x = gEditor.palette.bounds.x + cx;
        float y = gEditor.palette.bounds.y + cy;

        Rect bounds = { x,y,ICON_W,ICON_H };

        // If the mouse is hovering over the editor icon then apply some feedback effects.
        // We do this before applying scale so the bounding box doesn't get messed with.
        if (PointAndRectCollision(mouse, bounds))
        {
            if (t.draw.color.target != EDITOR_COLOR2) // Important for pressed effects to work properly!
            {
                t.draw.color.current = EDITOR_COLOR2;
            }
            t.draw.color.target = EDITOR_COLOR2;
            t.draw.scale.target = EDITOR_PALETTE_ICON_HOVER_SCALE;

            if (IsMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                t.draw.scale.current = EDITOR_PALETTE_ICON_HIT_SCALE;
                t.draw.angle.current = EDITOR_PALETTE_ICON_HIT_ANGLE;

                // Set the new selected!
                gEditor.cursor.selected.type = image;
                gEditor.cursor.selected.index = i;

                float ca = gEditor.cursor.draw.color.current.a;
                float ta = gEditor.cursor.draw.color.target.a;

                gEditor.cursor.draw.color.current = GetSelectedEditorIcon()->draw.color.base;
                gEditor.cursor.draw.color.target = GetSelectedEditorIcon()->draw.color.base;
                gEditor.cursor.draw.color.current.a = ca;
                gEditor.cursor.draw.color.target.a = ta;
            }
        }
        else
        {
            t.draw.color.target = t.draw.color.base;
            t.draw.scale.target = EDITOR_PALETTE_ICON_SCALE;
        }

        // Apply lerping effects.
        t.draw.color.current = Lerp(t.draw.color.current, t.draw.color.target, gApplication.delta_time*15);
        t.draw.scale.current = Lerp(t.draw.scale.current, t.draw.scale.target, gApplication.delta_time*30);
        t.draw.angle.current = Lerp(t.draw.angle.current, t.draw.angle.target, gApplication.delta_time*15);

        // Adjust the X and Y for the current scale of the icon so it remains centered.
        x -= (TILE_W * (t.draw.scale.current - EDITOR_PALETTE_ICON_SCALE)) / 2;
        y -= (TILE_H * (t.draw.scale.current - EDITOR_PALETTE_ICON_SCALE)) / 2;

        Vec2 scale = { t.draw.scale.current, t.draw.scale.current };
        Vec2 center = { (TILE_W*scale.x)/2, (TILE_H*scale.y)/2 };
        DrawImage(image, x,y, scale, center, t.draw.angle.current, FLIP_NONE, t.draw.color.current, &t.draw.clip);
        cx += ICON_W + EDITOR_PALETTE_ICON_PADDING;
    }
}

INTERNAL void DoEditorPalette ()
{
    float ww = (float)GetWindowWidth();
    float wh = (float)GetWindowHeight();

    gEditor.palette.bounds.x = ww - (EDITOR_PALETTE_WIDTH + EDITOR_PALETTE_BORDER);
    gEditor.palette.bounds.y = EDITOR_PALETTE_BORDER;
    gEditor.palette.bounds.w = EDITOR_PALETTE_WIDTH;
    gEditor.palette.bounds.h = wh - (EDITOR_PALETTE_BORDER * 2);

    DrawFill(gEditor.palette.bounds, EDITOR_COLOR0);

    // List all of the placeable tiles and entities.
    float cx = EDITOR_PALETTE_ICON_PADDING, cy = EDITOR_PALETTE_ICON_PADDING;
    DoEditorPaletteIcons(cx,cy, gEditor.tile_icons, "tile");
    cx = EDITOR_PALETTE_ICON_PADDING, cy += (TILE_H*EDITOR_PALETTE_ICON_SCALE) + (EDITOR_PALETTE_ICON_PADDING*3);
    DoEditorPaletteIcons(cx,cy, gEditor.entity_icons, "entity");
}

INTERNAL void DoEditorCanvas ()
{
    float ww = (float)GetWindowWidth();
    float wh = (float)GetWindowHeight();

    // Calculate how large we can make the chunk editor canvas within the available space.

    gEditor.canvas.space.x = 0.0f;
    gEditor.canvas.space.y = 0.0f;
    gEditor.canvas.space.w = ww - (ww - gEditor.palette.bounds.x);
    gEditor.canvas.space.h = wh;

    gEditor.canvas.bounds.w = (CHUNK_W * TILE_W);
    gEditor.canvas.bounds.h = (CHUNK_H * TILE_H);

    gEditor.canvas.scale = 1.0f;
    while (gEditor.canvas.bounds.w*(gEditor.canvas.scale+1) < gEditor.canvas.space.w &&
           gEditor.canvas.bounds.h*(gEditor.canvas.scale+1) < gEditor.canvas.space.h)
    {
        gEditor.canvas.scale++;
    }

    gEditor.canvas.bounds.w *=  gEditor.canvas.scale;
    gEditor.canvas.bounds.h *=  gEditor.canvas.scale;
    gEditor.canvas.bounds.x  = (gEditor.canvas.space.w-gEditor.canvas.bounds.w) / 2;
    gEditor.canvas.bounds.y  = (gEditor.canvas.space.h-gEditor.canvas.bounds.h) / 2;

    DrawFill(gEditor.canvas.bounds, EDITOR_COLOR0);

    // Handle and draw the tiles and entities in the current chunk.

    float tilew = (TILE_W*gEditor.canvas.scale);
    float tileh = (TILE_H*gEditor.canvas.scale);

    for (int iy=0; iy<CHUNK_H; ++iy)
    {
        for (int ix=0; ix<CHUNK_W; ++ix)
        {
            EditorThing& thing = gEditor.chunk.things[iy][ix];
            if (thing.active) // @Temporary: Don't draw anything if not active...
            {
                float x = gEditor.canvas.bounds.x + ((float)ix * tilew);
                float y = gEditor.canvas.bounds.y + ((float)iy * tileh);
                float scale = gEditor.canvas.scale * thing.draw.scale.current;
                Vec2 center = { 0,0 };
                DrawImage(thing.icon_index.type, x,y, {scale,scale}, center, thing.draw.angle.current, FLIP_NONE, thing.draw.color.current, &thing.draw.clip);
            }
        }
    }

    // Handle the mouse cursor drawing and functionality.

    Vec2 mouse = GetMousePos();

    int tilex = (int)floorf((mouse.x-gEditor.canvas.bounds.x) / tilew);
    int tiley = (int)floorf((mouse.y-gEditor.canvas.bounds.y) / tileh);

    gEditor.cursor.pos.x = tilex;
    gEditor.cursor.pos.y = tiley;

    if (PointAndRectCollision(mouse,gEditor.canvas.bounds))
    {
        gEditor.cursor.draw.color.target.a = EDITOR_CURSOR_ALPHA;

        // Handle adding and removing things from the current chunk.
        EditorThing& thing = gEditor.chunk.things[gEditor.cursor.pos.y][gEditor.cursor.pos.x];
        if (IsMouseButtonDown(SDL_BUTTON_LEFT))
        {
            // INSERT THING!!!
            EditorIcon* icon = GetSelectedEditorIcon();
            if (icon)
            {
                thing.icon_index = gEditor.cursor.selected;
                thing.active = true;

                thing.draw.clip = icon->draw.clip;
                thing.draw.scale.current = 1.0f;
                thing.draw.scale.target = 1.0f;
                thing.draw.angle.current = 0.0f;
                thing.draw.angle.target = 0.0f;
                thing.draw.color.current = icon->draw.color.base;
                thing.draw.color.target = icon->draw.color.base;
                thing.draw.color.base = icon->draw.color.base;
            }
        }
        else if (IsMouseButtonDown(SDL_BUTTON_RIGHT))
        {
            //
            // REMOVE THING
            //

            thing.active = false;
            // @Incomplete: Do effects...
        }
    }
    else
    {
        gEditor.cursor.draw.color.target.a = 0.0f;
    }

    // Lerp values for smooth effects.
    gEditor.cursor.draw.pos.x = Lerp(gEditor.cursor.draw.pos.x, ((float)tilex*tilew), gApplication.delta_time*20);
    gEditor.cursor.draw.pos.y = Lerp(gEditor.cursor.draw.pos.y, ((float)tiley*tileh), gApplication.delta_time*20);
    gEditor.cursor.draw.color.current.a = Lerp(gEditor.cursor.draw.color.current.a, gEditor.cursor.draw.color.target.a, gApplication.delta_time*40);

    EditorIcon* icon = GetSelectedEditorIcon();
    if (icon)
    {
        float x = gEditor.canvas.bounds.x + gEditor.cursor.draw.pos.x;
        float y = gEditor.canvas.bounds.y + gEditor.cursor.draw.pos.y;

        Vec2 scale = { gEditor.canvas.scale, gEditor.canvas.scale };
        Vec2 center = { 0,0 };

        DrawImage(gEditor.cursor.selected.type, x,y, scale, center, 0.0f, FLIP_NONE, gEditor.cursor.draw.color.current, &icon->draw.clip);
    }
}

INTERNAL void DoEditor ()
{
    DoEditorPalette();
    DoEditorCanvas();
}
