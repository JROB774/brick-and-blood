GLOBAL constexpr float PLAYER_INPUT_REFRESH_TIME = 0.1f;
GLOBAL constexpr int PLAYER_MAX_ITEM_STACK = 99;

INTERNAL void PlayerPickUpItem (std::string name, int amount)
{
    if (amount <= 0) return;

    // If the specified base type can't be found then we don't pick up an item
    if (!gItems.count(name))
    {
        LOG_ERROR(ERR_MIN, "Could not pickup item of unknown type: %s", name.c_str());
        return;
    }

    // If we already have the item in our inventory then add to it.
    InventoryItem* found = NULL;
    for (auto& item: gPlayer.inventory.items)
    {
        if (item.name == name)
        {
            found = &item;
            item.amount += amount;
            break;
        }
    }
    // Otherwise we create a new entry for it.
    if (!found)
    {
        gPlayer.inventory.items.push_back({ name, amount });
        found = &gPlayer.inventory.items.back();

        // If there is an available hotbar slot, then place it in there.
        for (int i=0; i<HOTBAR_SIZE; ++i)
        {
            if (gPlayer.hotbar.items[i] == HOTBAR_ITEM_EMPTY)
            {
                gPlayer.hotbar.items[i] = (int)gPlayer.inventory.items.size()-1;
                break;
            }
        }
    }

    // Make sure the number of items does not exceed the stack limit.
    if (found->amount > PLAYER_MAX_ITEM_STACK)
    {
        found->amount = PLAYER_MAX_ITEM_STACK;
    }
}

INTERNAL void InitPlayer ()
{
    gPlayer.state = PLAYER_STATE_PLAY;

    gPlayer.input_timer = 0.0f;
    gPlayer.update = false;

    gPlayer.inventory.items.clear();
    gPlayer.inventory.selected_item = 0;

    gPlayer.inventory.bounds.current = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };
    gPlayer.inventory.bounds.target = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };

    for (int i=0; i<HOTBAR_SIZE; ++i) gPlayer.hotbar.items[i] = HOTBAR_ITEM_EMPTY;
    gPlayer.hotbar.selected_item = 0;

    // Set the inital position of the camera.
    Entity* p = MapGetFirstEntityOfType("player");
    if (p)
    {
        float cx = roundf(p->draw.pos.x + (TILE_W/2) - (WINDOW_SCREEN_W/2));
        float cy = roundf(p->draw.pos.y + (TILE_H/2) - (WINDOW_SCREEN_H/2));
        SetCamera(cx,cy);
    }
}

INTERNAL void OpenInventory ()
{
    gPlayer.state = PLAYER_STATE_INVENTORY;
    gPlayer.input_timer = 0.0f;
    gPlayer.inventory.bounds.current = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };
    gPlayer.inventory.bounds.target = { TILE_W,TILE_H,WINDOW_SCREEN_W-(TILE_W*2),WINDOW_SCREEN_H-(TILE_H*2) };
    gPlayer.inventory.scale.current = { 0,0 };
    gPlayer.inventory.scale.target = { 1,1 };
}
INTERNAL void CloseInventory ()
{
    gPlayer.state = PLAYER_STATE_PLAY;
    gPlayer.input_timer = 0.0f;
    gPlayer.inventory.bounds.target = { WINDOW_SCREEN_W/2,WINDOW_SCREEN_H/2, 0,0 };
    gPlayer.inventory.scale.target = { 0,0 };
}

INTERNAL void UpdatePlayerStatePlay ()
{
    if (IsKeyPressed(SDL_SCANCODE_TAB))
    {
        OpenInventory();
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

    // Change the currently selected hotbar item.
    if (IsKeyPressed(SDL_SCANCODE_1)) gPlayer.hotbar.selected_item = 0;
    if (IsKeyPressed(SDL_SCANCODE_2)) gPlayer.hotbar.selected_item = 1;
    if (IsKeyPressed(SDL_SCANCODE_3)) gPlayer.hotbar.selected_item = 2;
    if (IsKeyPressed(SDL_SCANCODE_4)) gPlayer.hotbar.selected_item = 3;
    if (IsKeyPressed(SDL_SCANCODE_5)) gPlayer.hotbar.selected_item = 4;
    if (IsKeyPressed(SDL_SCANCODE_6)) gPlayer.hotbar.selected_item = 5;
    if (IsKeyPressed(SDL_SCANCODE_7)) gPlayer.hotbar.selected_item = 6;
    if (IsKeyPressed(SDL_SCANCODE_8)) gPlayer.hotbar.selected_item = 7;
    if (IsKeyPressed(SDL_SCANCODE_9)) gPlayer.hotbar.selected_item = 8;
}

INTERNAL void PlayerSetHotbarItemToSelected (int slot)
{
    if (gPlayer.hotbar.items[slot] == gPlayer.inventory.selected_item) // If the item is already in the slot then remove it.
    {
        gPlayer.hotbar.items[slot] = HOTBAR_ITEM_EMPTY;
    }
    else // Otherwise put it into the hotbar at that location.
    {
        gPlayer.hotbar.items[slot] = gPlayer.inventory.selected_item;
        for (int i=0; i<HOTBAR_SIZE; ++i) // Make sure no other slots are set to this item.
        {
            if (i != slot)
            {
                if (gPlayer.hotbar.items[i] == gPlayer.hotbar.items[slot])
                {
                    gPlayer.hotbar.items[i] = HOTBAR_ITEM_EMPTY;
                }
            }
        }
    }
}

INTERNAL void UpdatePlayerStateInventory ()
{
    if (IsKeyPressed(SDL_SCANCODE_TAB))
    {
        CloseInventory();
        return;
    }

    bool update = false;

    // If any of the player's input keys are pressed we update the simulation.
    if (IsKeyPressed(SDL_SCANCODE_W)  ||
        IsKeyPressed(SDL_SCANCODE_S)  ||
        IsKeyPressed(SDL_SCANCODE_UP) ||
        IsKeyPressed(SDL_SCANCODE_DOWN))
    {
        gPlayer.input_timer = PLAYER_INPUT_REFRESH_TIME;
        update = true;
    }
    // This system exists so that keys can be held.
    if (IsKeyDown(SDL_SCANCODE_W)  ||
        IsKeyDown(SDL_SCANCODE_S)  ||
        IsKeyDown(SDL_SCANCODE_UP) ||
        IsKeyDown(SDL_SCANCODE_DOWN))
    {
        gPlayer.input_timer -= gApplication.delta_time;
        if (gPlayer.input_timer <= 0.0f)
        {
            gPlayer.input_timer = PLAYER_INPUT_REFRESH_TIME;
            update = true;
        }
    }

    // INVENTORY
    if (update)
    {
        if (IsKeyDown(SDL_SCANCODE_W) || IsKeyDown(SDL_SCANCODE_UP))
        {
            gPlayer.inventory.selected_item--;
            if (gPlayer.inventory.selected_item < 0)
            {
                gPlayer.inventory.selected_item = (int)gPlayer.inventory.items.size()-1;
            }
        }
        if (IsKeyDown(SDL_SCANCODE_S) || IsKeyDown(SDL_SCANCODE_DOWN))
        {
            gPlayer.inventory.selected_item++;
            if (gPlayer.inventory.selected_item > gPlayer.inventory.items.size()-1)
            {
                gPlayer.inventory.selected_item = 0;
            }
        }

        gPlayer.inventory.selected_item = std::clamp(gPlayer.inventory.selected_item, 0, (int)gPlayer.inventory.items.size()-1);
    }
    if (!gPlayer.inventory.items.empty())
    {
        if (IsKeyPressed(SDL_SCANCODE_1)) PlayerSetHotbarItemToSelected(0);
        if (IsKeyPressed(SDL_SCANCODE_2)) PlayerSetHotbarItemToSelected(1);
        if (IsKeyPressed(SDL_SCANCODE_3)) PlayerSetHotbarItemToSelected(2);
        if (IsKeyPressed(SDL_SCANCODE_4)) PlayerSetHotbarItemToSelected(3);
        if (IsKeyPressed(SDL_SCANCODE_5)) PlayerSetHotbarItemToSelected(4);
        if (IsKeyPressed(SDL_SCANCODE_6)) PlayerSetHotbarItemToSelected(5);
        if (IsKeyPressed(SDL_SCANCODE_7)) PlayerSetHotbarItemToSelected(6);
        if (IsKeyPressed(SDL_SCANCODE_8)) PlayerSetHotbarItemToSelected(7);
        if (IsKeyPressed(SDL_SCANCODE_9)) PlayerSetHotbarItemToSelected(8);
    }

    // CRAFTING
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
}

INTERNAL void RenderPlayerHeadsUp ()
{
    const Vec4 HEADSUP_BG_COLOR = { 0.28f,0.18f,0.24f,1.0f };
    const Vec4 HEADSUP_FG_COLOR = SDLColorToColor({ 244,180,27,255 });

    if (gPlayer.state != PLAYER_STATE_INVENTORY)
    {
        DrawFill(1,1,137,18, HEADSUP_BG_COLOR);
        DrawImage("headsup", 1,1, {1,1}, {0,0}, 0.0f, FLIP_NONE, HEADSUP_FG_COLOR);

        // Draw the items in the hotbar.
        float x = 21;
        float y = 6;
        for (int i=0; i<HOTBAR_SIZE; ++i)
        {
            Vec4 color = HEADSUP_FG_COLOR;
            if (i == gPlayer.hotbar.selected_item)
            {
                DrawFill(x-1,y-1,10,10, HEADSUP_FG_COLOR);
                color = HEADSUP_BG_COLOR;
            }

            if (gPlayer.hotbar.items[i] != HOTBAR_ITEM_EMPTY)
            {
                InventoryItem& item = gPlayer.inventory.items.at(gPlayer.hotbar.items[i]);
                DrawImage("item", x,y, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, color, &GetItem(item.name).clip);

                // @Incomplete: Draw the quantity...
            }
            x += 13;
        }
    }
}

INTERNAL void RenderPlayerInventory ()
{
    const Vec4 INVENTORY_BG_COLOR = SDLColorToColor({ 38,13,28,255 });
    const Vec4 INVENTORY_FG_COLOR = SDLColorToColor({ 244,180,27,255 });

    const float INVENTORY_INSET = 3;

    const float INVENTORY_ITEM_BOUNDS_W = 96;
    const float INVENTORY_ITEM_BOUNDS_H = 186;

    const float INVENTORY_TEXT_OFF = 1;

    gPlayer.inventory.bounds.current = Lerp(gPlayer.inventory.bounds.current, gPlayer.inventory.bounds.target, gApplication.delta_time*30);
    gPlayer.inventory.scale.current = Lerp(gPlayer.inventory.scale.current, gPlayer.inventory.scale.target, gApplication.delta_time*30);

    DrawFill(gPlayer.inventory.bounds.current, INVENTORY_BG_COLOR);

    // Don't draw if the inventory is closed...
    if (gPlayer.inventory.bounds.current.x >= ((WINDOW_SCREEN_W/2)-INVENTORY_INSET) &&
        gPlayer.inventory.bounds.current.y >= ((WINDOW_SCREEN_H/2)-INVENTORY_INSET))
    {
        return;
    }

    // Draw the actual inventory.

    float ix = gPlayer.inventory.bounds.current.x;
    float iy = gPlayer.inventory.bounds.current.y;

    DrawImage("inventorybg", ix,iy, gPlayer.inventory.scale.current, {0,0}, 0.0f, FLIP_NONE, INVENTORY_FG_COLOR);

    if ((gPlayer.inventory.bounds.current.w >= gPlayer.inventory.bounds.target.w-3) && (gPlayer.inventory.bounds.target.w != 0.0f))
    {
        // INVENTORY
        ScissorOn(27,35,127,88);
        {
            float x = 27;
            float y = 35;

            int index = 0;
            for (auto item: gPlayer.inventory.items)
            {
                if (item.amount > 0)
                {
                    Vec4 color = INVENTORY_FG_COLOR;
                    if (index == gPlayer.inventory.selected_item)
                    {
                        DrawFill(x,y,128,8, INVENTORY_FG_COLOR);
                        color = INVENTORY_BG_COLOR;
                    }

                    DrawImage("item", x+INVENTORY_TEXT_OFF,y, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, color, &GetItem(item.name).clip);
                    DrawText("main", StrUpper(item.name), x+INVENTORY_TEXT_OFF+12,y, color);
                    std::string quant = std::to_string(item.amount);
                    DrawText("main", StrUpper(quant), (x+127)-(INVENTORY_TEXT_OFF+GetTextWidth("main",quant)),y, color);

                    y += 8;
                    index++;
                }
            }
        }
        ScissorOff();

        // CRAFTING
        ScissorOn(166,35,127,88);
        {
            // @Incomplete: ...
        }
        ScissorOff();

        // HOTBAR
        ScissorOn(27,131,266,26);
        {
            float x = 32;
            float y = 136;
            for (int i=0; i<HOTBAR_SIZE; ++i)
            {
                if (gPlayer.hotbar.items[i] != HOTBAR_ITEM_EMPTY)
                {
                    InventoryItem& item = gPlayer.inventory.items.at(gPlayer.hotbar.items[i]);
                    DrawImage("item", x,y, {1,1}, {0,0}, 0.0f, FLIP_NONE, INVENTORY_FG_COLOR, &GetItem(item.name).clip);
                }
                x += 30;
            }
        }
        ScissorOff();
    }

    DrawImage("inventoryfg", ix,iy, gPlayer.inventory.scale.current, {0,0}, 0.0f, FLIP_NONE, INVENTORY_FG_COLOR);
}

INTERNAL void RenderPlayer ()
{
    RenderPlayerHeadsUp();
    RenderPlayerInventory();
}
