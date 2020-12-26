GLOBAL constexpr float PLAYER_INPUT_REFRESH_TIME = 0.1f;

GLOBAL size_t gInventoryId = 1; // Unique ID!

INTERNAL InventoryItem* GetInventoryItemByID (size_t id)
{
    InventoryItem* item = NULL;
    for (auto& i: gPlayer.inventory.items)
    {
        if (id == i.id)
        {
            item = &i;
        }
    }
    return item;
}
INTERNAL InventoryItem* GetInventoryItemByName (std::string name)
{
    InventoryItem* item = NULL;
    for (auto& i: gPlayer.inventory.items)
    {
        if (name == i.name)
        {
            item = &i;
        }
    }
    return item;
}

INTERNAL std::string PlayerGetEquippedItemName ()
{
    size_t id = gPlayer.hotbar.items[gPlayer.hotbar.selected_item];
    if (id == HOTBAR_ITEM_EMPTY) return "";
    return GetInventoryItemByID(id)->name;
}

INTERNAL void PlayerRefreshInventory ()
{
    // Refresh all the items and crafting recipes displayed on the inventory screen.

    // INVENTORY
    {
        // Update the hotbar to remove any items that are empty (NEEDS TO GO FIRST).
        for (int i=0; i<HOTBAR_SIZE; ++i)
        {
            if (gPlayer.hotbar.items[i] != HOTBAR_ITEM_EMPTY)
            {
                InventoryItem* item = GetInventoryItemByID(gPlayer.hotbar.items[i]);
                if (item->amount <= 0) gPlayer.hotbar.items[i] = HOTBAR_ITEM_EMPTY;
            }
        }

        // Remove any items that have been emptied out.
        int old_size = (int)gPlayer.inventory.items.size();
        gPlayer.inventory.items.erase(std::remove_if(gPlayer.inventory.items.begin(),gPlayer.inventory.items.end(),
        [=](const InventoryItem& i) { return (i.amount <= 0); }),
        gPlayer.inventory.items.end());

        // Sort the inventory based on category.
        std::stable_sort(gPlayer.inventory.items.begin(), gPlayer.inventory.items.end(),
        [](const InventoryItem& a, const InventoryItem& b)
        {
            auto& ia = GetItem(a.name);
            auto& ib = GetItem(b.name);
            if (ia.category == ib.category) return (a.name < b.name);
            return (ia.category < ib.category);
        });

        // Make sure the selected item is still in range.
        gPlayer.inventory.selected_item = std::clamp(gPlayer.inventory.selected_item, 0, (int)gPlayer.inventory.items.size()-1);
    }

    // CRAFTING
    {
        gPlayer.inventory.recipes.clear();

        // Check if new recipes can be made and add them to the list.
        for (auto& [name,item]: gItems)
        {
            if (!item.recipe.ingredients.empty())
            {
                bool available = true;
                if (!item.recipe.equipment.empty()) // Some recipes require special equipment.
                {
                    available = false;
                    auto tiles = MapGetAllLoadedTilesOfType(item.recipe.equipment);
                    if (!tiles.empty())
                    {
                        Entity* p = MapGetFirstEntityOfType("player");
                        if (p) // We need the player to check distance!
                        {
                            for (auto& t: tiles) // Check how close and if in range of the player, the recipe is available.
                            {
                                if (abs(Distance(t->pos.x,t->pos.y, p->pos.x,p->pos.y)) <= EQUIPMENT_CRAFT_DISTANCE)
                                {
                                    available = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (available) // Check for the actual ingredients being present.
                {
                    for (auto& ingredient: item.recipe.ingredients)
                    {
                        InventoryItem* ii = GetInventoryItemByName(ingredient.type);
                        if (!ii || ii->amount < ingredient.amount)
                        {
                            available = false;
                            break;
                        }
                    }
                }
                if (available)
                {
                    gPlayer.inventory.recipes.push_back(name);
                }
            }
        }

        // Sort the recipes based on category.
        std::stable_sort(gPlayer.inventory.recipes.begin(), gPlayer.inventory.recipes.end(),
        [](const std::string& a, const std::string& b)
        {
            auto& ia = GetItem(a);
            auto& ib = GetItem(b);
            if (ia.category == ib.category) return (a < b);
            return (ia.category < ib.category);
        });

        // Make sure the selected item is still in range.
        gPlayer.inventory.selected_recipe = std::clamp(gPlayer.inventory.selected_recipe, 0, (int)gPlayer.inventory.recipes.size()-1);

        // If there are no recipes make sure the state is set to the item list instead.
        if (gPlayer.inventory.recipes.empty())
        {
            gPlayer.inventory.state = INVENTORY_STATE_ITEMS;
        }
    }
}

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
        gPlayer.inventory.items.push_back({ gInventoryId++, name, amount });
        found = &gPlayer.inventory.items.back();

        // If there is an available hotbar slot, then place it in there.
        for (int i=0; i<HOTBAR_SIZE; ++i)
        {
            if (gPlayer.hotbar.items[i] == HOTBAR_ITEM_EMPTY)
            {
                gPlayer.hotbar.items[i] = gPlayer.inventory.items.back().id;
                break;
            }
        }
    }

    // Make sure the number of items does not exceed the stack limit.
    if (found->amount > GetItem(name).stack)
    {
        found->amount = GetItem(name).stack;
    }

    PlayerRefreshInventory();
}

INTERNAL void PlayerEatSelectedItem ()
{
    // Important to discard the fractional value by rounding upwards (so it matches the display hunger).
    if (ceilf(gPlayer.hunger) >= PLAYER_MAX_HUNGER) return; // Do not eat if not hungry...

    size_t item_id = gPlayer.hotbar.items[gPlayer.hotbar.selected_item];
    if (item_id == HOTBAR_ITEM_EMPTY) return;

    InventoryItem* item = GetInventoryItemByID(item_id);
    if (item)
    {
        float hunger = GetItem(item->name).hunger;
        if (hunger > 0)
        {
            gPlayer.hunger = std::clamp(gPlayer.hunger+hunger, 0.0f,PLAYER_MAX_HUNGER);
            item->amount--;
            if (item->amount <= 0)
            {
                PlayerRefreshInventory();
            }
        }
    }
}

INTERNAL void PlayerPlaceSelectedItem (int x, int y)
{
    size_t item_id = gPlayer.hotbar.items[gPlayer.hotbar.selected_item];
    if (item_id == HOTBAR_ITEM_EMPTY) return;

    InventoryItem* item = GetInventoryItemByID(item_id);
    if (item)
    {
        std::string place = GetItem(item->name).place;
        if (!place.empty()) // If the item can be placed.
        {
            Tile* t = MapGetTileAtPos(x,y);
            if (t && (!t->active || t->hits == TILE_INDESTRUCTIBLE))
            {
                MapSpawnTile(place,x,y);
                item->amount--;
                if (item->amount <= 0)
                {
                    PlayerRefreshInventory();
                }
            }
        }
    }
}

INTERNAL void DamagePlayer (int damage)
{
    gPlayer.health -= damage;
    Entity* p = MapGetFirstEntityOfType("player");
    if (p) DamageEntity(*p, damage);
}

INTERNAL void InitPlayer ()
{
    gPlayer.state = PLAYER_STATE_PLAY;

    gPlayer.health = PLAYER_MAX_HEALTH;
    gPlayer.hunger = PLAYER_MAX_HUNGER;

    gPlayer.input_timer = 0.0f;
    gPlayer.update = false;

    gPlayer.inventory.state = INVENTORY_STATE_ITEMS;

    gPlayer.inventory.items.clear();
    gPlayer.inventory.selected_item = 0;

    gPlayer.inventory.recipes.clear();
    gPlayer.inventory.selected_recipe = 0;

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
    PlayerRefreshInventory();
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
    if (IsKeyPressed(SDL_SCANCODE_A    ) ||
        IsKeyPressed(SDL_SCANCODE_D    ) ||
        IsKeyPressed(SDL_SCANCODE_W    ) ||
        IsKeyPressed(SDL_SCANCODE_S    ) ||
        IsKeyPressed(SDL_SCANCODE_UP   ) ||
        IsKeyPressed(SDL_SCANCODE_RIGHT) ||
        IsKeyPressed(SDL_SCANCODE_DOWN ) ||
        IsKeyPressed(SDL_SCANCODE_LEFT ) ||
        IsKeyPressed(SDL_SCANCODE_SPACE))
    {
        gPlayer.input_timer = PLAYER_INPUT_REFRESH_TIME * 3; // Initial cooldown is longer...
        gPlayer.update = true;
    }
    // This system exists so that keys can be held.
    if (IsKeyDown(SDL_SCANCODE_A    ) ||
        IsKeyDown(SDL_SCANCODE_D    ) ||
        IsKeyDown(SDL_SCANCODE_W    ) ||
        IsKeyDown(SDL_SCANCODE_S    ) ||
        IsKeyDown(SDL_SCANCODE_UP   ) ||
        IsKeyDown(SDL_SCANCODE_RIGHT) ||
        IsKeyDown(SDL_SCANCODE_DOWN ) ||
        IsKeyDown(SDL_SCANCODE_LEFT ) ||
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

    // Handle code on the player performing an action / updating.
    if (gPlayer.update)
    {
        // Increment the game's turn counter.
        gGameState.turns++;

        // If the player has performed an action then make them hungry.
        gPlayer.hunger -= PLAYER_HUNGER_DECREMENT;
        // If their hunger is high then heal the player.
        if (gPlayer.hunger >= PLAYER_HUNGER_GREAT)
        {
            gPlayer.health += PLAYER_HEALTH_INCREMENT*6;
        }
        else if (gPlayer.hunger >= PLAYER_HUNGER_HIGH)
        {
            gPlayer.health += PLAYER_HEALTH_INCREMENT;
        }
        // If their hunger is low then damage the player.
        if (gPlayer.hunger <= PLAYER_HUNGER_LOW)
        {
            gPlayer.health -= PLAYER_HEALTH_DECREMENT;
            if (gPlayer.hunger <= PLAYER_HUNGER_CRITICAL)
            {
                gPlayer.health -= PLAYER_HEALTH_DECREMENT*2;
                if (gPlayer.hunger <= 0.0f)
                {
                    gPlayer.health -= PLAYER_HEALTH_DECREMENT*2;
                }
            }
        }
        gPlayer.hunger = std::clamp(gPlayer.hunger, 0.0f,PLAYER_MAX_HUNGER);
        gPlayer.health = std::clamp(gPlayer.health, 0.0f,PLAYER_MAX_HEALTH);
    }
}

INTERNAL void PlayerSetHotbarItemToSelected (int slot)
{
    if (gPlayer.hotbar.items[slot] == gPlayer.inventory.items[gPlayer.inventory.selected_item].id) // If the item is already in the slot then remove it.
    {
        gPlayer.hotbar.items[slot] = HOTBAR_ITEM_EMPTY;
    }
    else // Otherwise put it into the hotbar at that location.
    {
        gPlayer.hotbar.items[slot] = gPlayer.inventory.items[gPlayer.inventory.selected_item].id;
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

INTERNAL void PlayerCraftItem (std::string item_name)
{
    PlayerPickUpItem(item_name, 1);

    // Remove the items that were used to craft.
    auto& recipe = GetItem(item_name).recipe;
    for (auto& ingredient: recipe.ingredients)
    {
        for (auto& item: gPlayer.inventory.items)
        {
            if (item.name == ingredient.type)
            {
                item.amount -= ingredient.amount;
                break;
            }
        }
    }

    PlayerRefreshInventory();
}

INTERNAL void UpdatePlayerStateInventory ()
{
    // @Incomplete: Need to handle scrolling the inventory and crafting lists when there are too many items...

    if (IsKeyPressed(SDL_SCANCODE_TAB))
    {
        CloseInventory();
        return;
    }

    bool update = false;

    // If any of the player's input keys are pressed we update the simulation.
    if (IsKeyPressed(SDL_SCANCODE_W)    ||
        IsKeyPressed(SDL_SCANCODE_S)    ||
        IsKeyPressed(SDL_SCANCODE_UP)   ||
        IsKeyPressed(SDL_SCANCODE_DOWN) ||
        IsKeyPressed(SDL_SCANCODE_SPACE))
    {
        gPlayer.input_timer = PLAYER_INPUT_REFRESH_TIME * 3; // Initial cooldown is longer...
        update = true;
    }
    // This system exists so that keys can be held.
    if (IsKeyDown(SDL_SCANCODE_W)    ||
        IsKeyDown(SDL_SCANCODE_S)    ||
        IsKeyDown(SDL_SCANCODE_UP)   ||
        IsKeyDown(SDL_SCANCODE_DOWN) ||
        IsKeyDown(SDL_SCANCODE_SPACE))
    {
        gPlayer.input_timer -= gApplication.delta_time;
        if (gPlayer.input_timer <= 0.0f)
        {
            gPlayer.input_timer = PLAYER_INPUT_REFRESH_TIME;
            update = true;
        }
    }

    // Set state to and from inventory and crafting.
    if (IsKeyPressed(SDL_SCANCODE_A) || IsKeyPressed(SDL_SCANCODE_LEFT))
    {
        if (!gPlayer.inventory.items.empty())
        {
            gPlayer.inventory.state = INVENTORY_STATE_ITEMS;
        }
    }
    if (IsKeyPressed(SDL_SCANCODE_D) || IsKeyPressed(SDL_SCANCODE_RIGHT))
    {
        if (!gPlayer.inventory.recipes.empty())
        {
            gPlayer.inventory.state = INVENTORY_STATE_CRAFTING;
        }
    }

    // Handle update logic based on what state the inventory is currently in.
    switch (gPlayer.inventory.state)
    {
        // INVENTORY
        case (INVENTORY_STATE_ITEMS):
        {
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
        } break;
        // CRAFTING
        case (INVENTORY_STATE_CRAFTING):
        {
            if (update)
            {
                if (IsKeyDown(SDL_SCANCODE_W) || IsKeyDown(SDL_SCANCODE_UP))
                {
                    gPlayer.inventory.selected_recipe--;
                    if (gPlayer.inventory.selected_recipe < 0)
                    {
                        gPlayer.inventory.selected_recipe = (int)gPlayer.inventory.recipes.size()-1;
                    }
                }
                if (IsKeyDown(SDL_SCANCODE_S) || IsKeyDown(SDL_SCANCODE_DOWN))
                {
                    gPlayer.inventory.selected_recipe++;
                    if (gPlayer.inventory.selected_recipe > gPlayer.inventory.recipes.size()-1)
                    {
                        gPlayer.inventory.selected_recipe = 0;
                    }
                }

                gPlayer.inventory.selected_recipe = std::clamp(gPlayer.inventory.selected_recipe, 0, (int)gPlayer.inventory.recipes.size()-1);

                if (IsKeyDown(SDL_SCANCODE_SPACE))
                {
                    // Only craft if the player doesn't have the maximum stack of this item.
                    std::string item_name = gPlayer.inventory.recipes[gPlayer.inventory.selected_recipe];
                    InventoryItem* item = GetInventoryItemByName(item_name);
                    if (!item || item->amount < GetItem(item_name).stack)
                    {
                        PlayerCraftItem(item_name);
                    }
                }
            }

        } break;
    }
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

    // Go to the game over if the player dies.
    if (gPlayer.health <= 0.0f)
    {
        StartLose();
    }
}

INTERNAL void RenderPlayerHeadsUp ()
{
    const Vec4 HEADSUP_BG_COLOR = { 0.28f,0.18f,0.24f,1.0f };
    const Vec4 HEADSUP_FG_COLOR = SDLColorToColor({ 244,180,27,255 });

    if (gPlayer.state != PLAYER_STATE_INVENTORY)
    {
        const SDL_Rect MAIN_CLIP = { 0,0,156,22 };

        const SDL_Rect HEALTH_CLIP = { 0,22,16,16 };
        const SDL_Rect HUNGER_CLIP = { 16,22,16,16 };

        DrawFill(1,1,156,22, HEADSUP_BG_COLOR);

        DrawImage("headsup", 1, 1, {1.0f,1.0f}, {0,0}, 0.0f, FLIP_NONE, HEADSUP_FG_COLOR,   &MAIN_CLIP);
        DrawImage("headsup", 4, 4, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, HEADSUP_FG_COLOR, &HEALTH_CLIP);
        DrawImage("headsup", 4,12, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, HEADSUP_FG_COLOR, &HUNGER_CLIP);

        std::string health = std::to_string((int)ceilf(gPlayer.health));
        std::string hunger = std::to_string((int)ceilf(gPlayer.hunger));

        float tx,ty;

        tx = 14 + ((float)(3 - health.length()) * 8);
        ty = 4;
        DrawText("main", health, tx,ty, HEADSUP_FG_COLOR);
        tx = 14 + ((float)(3 - hunger.length()) * 8);
        ty = 12;
        DrawText("main", hunger, tx,ty, HEADSUP_FG_COLOR);

        // Draw the items in the hotbar.
        float x = 40;
        float y = 6;
        for (int i=0; i<HOTBAR_SIZE; ++i)
        {
            if (i == gPlayer.hotbar.selected_item)
            {
                DrawFill(x,y,8,8, HEADSUP_FG_COLOR);
            }

            if (gPlayer.hotbar.items[i] != HOTBAR_ITEM_EMPTY)
            {
                InventoryItem* item = GetInventoryItemByID(gPlayer.hotbar.items[i]);

                Vec4 color = GetItem(item->name).color;
                if (i == gPlayer.hotbar.selected_item)
                {
                    DrawFill(x,y,8,8, HEADSUP_FG_COLOR);
                    color = HEADSUP_BG_COLOR;
                }

                DrawImage("item", x,y, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, color, &GetItem(item->name).clip);

                // Draw the quantity.
                std::string quant = std::to_string(item->amount);
                tx = x+4-((GetTextWidth("main",quant)*0.5f)/2);
                ty = y+10;
                DrawText("main", StrUpper(quant), tx,ty, HEADSUP_BG_COLOR, 0.5f);
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

            int start_pos = (gPlayer.inventory.selected_item / 11) * 11; // 11 is the max number of items that can be displayed on screen at once.
            int end_pos = std::min(start_pos+11, (int)gPlayer.inventory.items.size());

            for (int index=start_pos; index<end_pos; ++index)
            {
                auto& item = gPlayer.inventory.items.at(index);

                if (item.amount > 0)
                {
                    // If selected then highlight.
                    Vec4 color = INVENTORY_FG_COLOR;
                    if (gPlayer.inventory.state == INVENTORY_STATE_ITEMS)
                    {
                        if (index == gPlayer.inventory.selected_item)
                        {
                            DrawFill(x,y,128,8, INVENTORY_FG_COLOR);
                            color = INVENTORY_BG_COLOR;
                        }
                    }

                    DrawImage("item", x+INVENTORY_TEXT_OFF,y, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, color, &GetItem(item.name).clip);
                    DrawText("main", StrUpper(GetItem(item.name).pretty_name), x+INVENTORY_TEXT_OFF+12,y, color);
                    std::string quant = std::to_string(item.amount);
                    DrawText("main", StrUpper(quant), (x+127)-(INVENTORY_TEXT_OFF+GetTextWidth("main",quant)),y, color);

                    y += 8;
                }
            }
        }
        ScissorOff();

        // CRAFTING
        ScissorOn(166,35,127,88);
        {
            float x = 166;
            float y = 35;

            int start_pos = (gPlayer.inventory.selected_recipe / 11) * 11; // 11 is the max number of items that can be displayed on screen at once.
            int end_pos = std::min(start_pos+11, (int)gPlayer.inventory.recipes.size());

            for (int index=start_pos; index<end_pos; ++index)
            {
                auto& item = gPlayer.inventory.recipes.at(index);

                // Grey out the item if the stack is full and it can't be crafted.
                InventoryItem* ii = GetInventoryItemByName(item);
                Vec4 color = INVENTORY_FG_COLOR;
                if (ii && ii->amount >= GetItem(item).stack)
                {
                    color = SDLColorToColor({ 114,109,102,255 });
                }

                // If selected then highlight.
                if (gPlayer.inventory.state == INVENTORY_STATE_CRAFTING)
                {
                    if (index == gPlayer.inventory.selected_recipe)
                    {
                        DrawFill(x,y,128,8, color);
                        color = INVENTORY_BG_COLOR;
                    }
                }

                DrawImage("item", x+INVENTORY_TEXT_OFF,y, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, color, &GetItem(item).clip);
                DrawText("main", StrUpper(GetItem(item).pretty_name), x+INVENTORY_TEXT_OFF+12,y, color);

                // Write out the recipe.
                float rx = (x+127)-INVENTORY_TEXT_OFF;
                auto& recipe = GetItem(item).recipe;
                for (int i=(int)recipe.ingredients.size()-1; i>=0; --i) // We render from right to left.
                {
                    ItemIngredient ingredient = recipe.ingredients.at(i);
                    std::string quant = std::to_string(ingredient.amount);
                    rx -= GetTextWidth("main",quant);
                    DrawText("main", StrUpper(quant), rx,y, color);
                    rx -= 8;
                    DrawImage("item", rx,y, {0.5f,0.5f}, {0,0}, 0.0f, FLIP_NONE, color, &GetItem(ingredient.type).clip);
                }

                y += 8;
            }
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
                    InventoryItem* item = GetInventoryItemByID(gPlayer.hotbar.items[i]);
                    DrawImage("item", x,y, {1,1}, {0,0}, 0.0f, FLIP_NONE, GetItem(item->name).color, &GetItem(item->name).clip);
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
