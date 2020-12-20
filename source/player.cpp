GLOBAL constexpr float PLAYER_INPUT_REFRESH_TIME = 0.1f;

INTERNAL void PlayerPickUp (std::string item, int amount)
{
    if (gPlayer.inventory.items.count(item)) gPlayer.inventory.items[item] += amount;
    else gPlayer.inventory.items[item] = amount;
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

INTERNAL void RenderPlayer ()
{
    // We do this here so it always happens (keeps things smooth).

    const Vec4 PLAYER_INVENTORY_BG_COLOR = SDLColorToColor({ 38,13,28,255 });
    const Vec4 PLAYER_INVENTORY_FG_COLOR = SDLColorToColor({ 244,180,27,255 });

    gPlayer.inventory.bounds.current = Lerp(gPlayer.inventory.bounds.current, gPlayer.inventory.bounds.target, gApplication.delta_time*30);
    DrawFill(gPlayer.inventory.bounds.current, PLAYER_INVENTORY_BG_COLOR);

    ScissorOn(gPlayer.inventory.bounds.current);

    // Draw the outline.
    float ox = gPlayer.inventory.bounds.current.x + 3;
    float oy = gPlayer.inventory.bounds.current.y + 3;
    float ow = gPlayer.inventory.bounds.current.w - 6;
    float oh = gPlayer.inventory.bounds.current.h - 6;
    DrawRect(ox,oy,ow,oh, PLAYER_INVENTORY_FG_COLOR);

    // @Incomplete: Draw the inventory...

    ScissorOff();
}
