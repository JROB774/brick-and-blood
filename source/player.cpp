GLOBAL constexpr float PLAYER_INPUT_REFRESH_TIME = 0.1f;
GLOBAL constexpr int PLAYER_MAX_ITEM_STACK = 999;

INTERNAL void PlayerPickUp (std::string item, int amount)
{
    if (gPlayer.inventory.items.count(item)) gPlayer.inventory.items[item] += amount;
    else gPlayer.inventory.items[item] = amount;
    // Make sure the number of items does not exceed the stack limit.
    if (gPlayer.inventory.items[item] > PLAYER_MAX_ITEM_STACK)
    {
        gPlayer.inventory.items[item] = PLAYER_MAX_ITEM_STACK;
    }
}

INTERNAL void InitPlayer ()
{
    gPlayer.state = PLAYER_STATE_PLAY;

    gPlayer.input_timer = 0.0f;
    gPlayer.update = false;

    gPlayer.inventory.items.clear();

    gPlayer.inventory.bounds.current = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };
    gPlayer.inventory.bounds.target = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };

    // Set the inital position of the camera.
    Entity* p = MapGetFirstEntityOfType("player");
    if (p)
    {
        float cx = roundf(p->draw.pos.x + (TILE_W/2) - (WINDOW_SCREEN_W/2));
        float cy = roundf(p->draw.pos.y + (TILE_H/2) - (WINDOW_SCREEN_H/2));
        SetCamera(cx,cy);
    }
}

INTERNAL void UpdatePlayerStatePlay ()
{
    if (IsKeyPressed(SDL_SCANCODE_TAB))
    {
        gPlayer.state = PLAYER_STATE_INVENTORY;
        gPlayer.inventory.bounds.current = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };
        gPlayer.inventory.bounds.target = { TILE_W,TILE_H,WINDOW_SCREEN_W-(TILE_W*2),WINDOW_SCREEN_H-(TILE_H*2) };
        return;
    }

    // If any of the player's input keys are pressed we update the simulation.
    if (IsKeyPressed(SDL_SCANCODE_A) ||
        IsKeyPressed(SDL_SCANCODE_D) ||
        IsKeyPressed(SDL_SCANCODE_W) ||
        IsKeyPressed(SDL_SCANCODE_S) ||
        IsKeyPressed(SDL_SCANCODE_SPACE))
    {
        gPlayer.input_timer = PLAYER_INPUT_REFRESH_TIME * 3; // Initial cooldown is longer...
        gPlayer.update = true;
    }
    // This system exists so that keys can be held.
    if (IsKeyDown(SDL_SCANCODE_A) ||
        IsKeyDown(SDL_SCANCODE_D) ||
        IsKeyDown(SDL_SCANCODE_W) ||
        IsKeyDown(SDL_SCANCODE_S) ||
        IsKeyDown(SDL_SCANCODE_SPACE))
    {
        gPlayer.input_timer -= gApplication.delta_time;
        if (gPlayer.input_timer <= 0.0f)
        {
            gPlayer.input_timer = PLAYER_INPUT_REFRESH_TIME;
            gPlayer.update = true;
        }
    }
}

INTERNAL void UpdatePlayerStateInventory ()
{
    if (IsKeyPressed(SDL_SCANCODE_TAB))
    {
        gPlayer.state = PLAYER_STATE_PLAY;
        gPlayer.inventory.bounds.target = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };
        return;
    }

    // @Incomplete: ...
}

INTERNAL void UpdatePlayer ()
{
    gPlayer.update = false;

    // Update player based on the current state.
    switch (gPlayer.state)
    {
        case (PLAYER_STATE_PLAY): UpdatePlayerStatePlay(); break;
        case (PLAYER_STATE_INVENTORY): UpdatePlayerStateInventory(); break;
    }

    // The camera tracks the player's current position!
    Entity* p = MapGetFirstEntityOfType("player");
    if (p)
    {
        float cx = roundf(p->draw.pos.x + (TILE_W/2) - (WINDOW_SCREEN_W/2));
        float cy = roundf(p->draw.pos.y + (TILE_H/2) - (WINDOW_SCREEN_H/2));
        SetCameraTarget(cx,cy);
    }

    // Debug key for printing the player inventory.
    #if defined(BUILD_DEBUG)
    if (IsKeyPressed(SDL_SCANCODE_I))
    {
        printf("INVENTORY:\n");
        for (auto [item,amount]: gPlayer.inventory.items)
        {
            printf("  %d %s\n", amount, item.c_str());
        }
    }
    #endif // BUILD_DEBUG
}

INTERNAL void RenderPlayerCursor ()
{
    const Vec4 FILL_COLOR = SDLColorToColor({ 244,180,27,255 });
    const Vec4 OUTLINE_COLOR = SDLColorToColor({ 38,13,28,255 });

    Vec2 pos = GetMousePos();

    int scale_x = GetWindowWidth() / WINDOW_SCREEN_W;
    int scale_y = GetWindowHeight() / WINDOW_SCREEN_H;
    int scale   = std::min(scale_x,scale_y);

    pos.x = (pos.x-GetViewport().x) / scale;
    pos.y = (pos.y-GetViewport().y) / scale;

    DrawImage("cursor", pos.x,pos.y, {1,1}, {0,0}, 0.0f, FLIP_NONE, FILL_COLOR);
    DrawImage("cursor_outline", pos.x,pos.y, {1,1}, {0,0}, 0.0f, FLIP_NONE, OUTLINE_COLOR);
}

INTERNAL void RenderPlayerInventory ()
{
    const Vec4 INVENTORY_BG_COLOR = SDLColorToColor({ 38,13,28,255 });
    const Vec4 INVENTORY_FG_COLOR = SDLColorToColor({ 244,180,27,255 });

    const float INVENTORY_INSET = 3;

    const float INVENTORY_ITEM_BOUNDS_W = 96;
    const float INVENTORY_ITEM_BOUNDS_H = 186;

    gPlayer.inventory.bounds.current = Lerp(gPlayer.inventory.bounds.current, gPlayer.inventory.bounds.target, gApplication.delta_time*30);
    DrawFill(gPlayer.inventory.bounds.current, INVENTORY_BG_COLOR);

    // Don't draw if the inventory is closed...
    if (gPlayer.inventory.bounds.current.x >= ((WINDOW_SCREEN_W/2)-INVENTORY_INSET) &&
        gPlayer.inventory.bounds.current.y >= ((WINDOW_SCREEN_H/2)-INVENTORY_INSET))
    {
        return;
    }

    // Draw the outline.
    float x = gPlayer.inventory.bounds.current.x + (INVENTORY_INSET  );
    float y = gPlayer.inventory.bounds.current.y + (INVENTORY_INSET  );
    float w = gPlayer.inventory.bounds.current.w - (INVENTORY_INSET*2);
    float h = gPlayer.inventory.bounds.current.h - (INVENTORY_INSET*2);
    DrawRect(x,y,w,h, INVENTORY_FG_COLOR);

    // This is the bounds you can draw in!
    x = roundf(x + ((INVENTORY_INSET+1)  ));
    y = roundf(y + ((INVENTORY_INSET+1)  ));
    w = roundf(w - ((INVENTORY_INSET+1)*2));
    h = roundf(h - ((INVENTORY_INSET+1)*2));

    ScissorOn(x,y,w,h);

    // Draw the item list.
    float ix = x;
    float iy = y;
    float iw = w/2;
    float ih = h;

    DrawRect(ix,iy,iw,ih, INVENTORY_FG_COLOR);

    ScissorOff();
}

INTERNAL void RenderPlayer ()
{
    RenderPlayerInventory();
    if (gPlayer.state == PLAYER_STATE_INVENTORY)
    {
        RenderPlayerCursor();
    }
}
