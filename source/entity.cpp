GLOBAL std::map<std::string,EntityBase> gEntities;

INTERNAL std::vector<std::string> GetAllEntityTypesOfFaction (std::string faction)
{
    std::vector<std::string> types;
    for (auto [type,base]: gEntities)
    {
        if (base.faction == faction)
        {
            types.push_back(type);
        }
    }
    return types;
}

INTERNAL void InitEntities ()
{
    gEntities.clear();

    GonObject& gon = *GetAsset<AssetData>("entities");
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Entity data not formatted as object!");
        return;
    }

    // Go through each base entity type and cache the data.
    for (auto& data: gon.children_array)
    {
        // Warn of entities with duplicate names.
        if (gEntities.count(data.name))
        {
            LOG_ERROR(ERR_MIN, "Warning duplicate entity with the name %s!", data.name.c_str());
        }

        EntityBase base = {};

        base.pretty_name = data["pretty_name"].String();
        base.faction = data["faction"].String();
        base.state = data["state"].String("");
        base.initiative = data["initiative"].Int(INT_MAX);
        base.health = data["health"].Int(ENTITY_INVINCIBLE);
        base.damage = data["damage"].Int(0);
        base.light = data["light"].Bool(false);
        base.light_radius = (float)data["light_radius"].Number(0.0);

        base.behavior_type = data["behavior"].String("none");
        if (base.behavior_type != "none")
        {
            if (ENTITY_BEHAVIOR.count(base.behavior_type)) base.behavior = ENTITY_BEHAVIOR.at(base.behavior_type);
            else LOG_ERROR(ERR_MIN, "No known entity behavior: %s", base.behavior_type.c_str());
        }

        if (data.Contains("image"))
        {
            base.image.x = data["image"][0].Int(0);
            base.image.y = data["image"][1].Int(0);
        }

        if (data.Contains("color"))
        {
            base.color.r = (float)data["color"][0].Number(255.0) / 255.0f;
            base.color.g = (float)data["color"][1].Number(255.0) / 255.0f;
            base.color.b = (float)data["color"][2].Number(255.0) / 255.0f;
            base.color.a = (float)data["color"][3].Number(255.0) / 255.0f;
        }
        else
        {
            base.color = { 1,1,1,1 };
        }

        if (data.Contains("particle_hit"))
        {
            if (data["particle_hit"].type == GonObject::FieldType::ARRAY)
            {
                for (int i=0; i<data["particle_hit"].size(); ++i)
                {
                    base.particle_hit.push_back(data["particle_hit"][i].String());
                }
            }
            else
            {
                base.particle_hit.push_back(data["particle_hit"].String());
            }
        }
        if (data.Contains("particle_dead"))
        {
            if (data["particle_dead"].type == GonObject::FieldType::ARRAY)
            {
                for (int i=0; i<data["particle_dead"].size(); ++i)
                {
                    base.particle_dead.push_back(data["particle_dead"][i].String());
                }
            }
            else
            {
                base.particle_dead.push_back(data["particle_dead"].String());
            }
        }

        if (data.Contains("sound_hit"))
        {
            if (data["sound_hit"].type == GonObject::FieldType::ARRAY)
            {
                for (int i=0; i<data["sound_hit"].size(); ++i)
                {
                    base.sound_hit.push_back(data["sound_hit"][i].String());
                }
            }
            else
            {
                base.sound_hit.push_back(data["sound_hit"].String());
            }
        }

        if (data.Contains("drops"))
        {
            for (int i=0; i<data["drops"].size(); ++i)
            {
                EntityDrops drops;
                drops.type = data["drops"][i][0].String();
                drops.min  = data["drops"][i][1].Int();
                drops.max  = data["drops"][i][2].Int();
                base.drops.push_back(drops);
            }
        }

        gEntities.insert({ data.name, base });
    }
}

INTERNAL void UpdateEntities (std::vector<Entity>& entities)
{
    // Sort the entities based on initiative so they get updated in correct order.
    std::sort(entities.begin(), entities.end());

    // Remove any entities that are no longer alive.
    entities.erase(std::remove_if(entities.begin(), entities.end(), [](const Entity& e){ return !e.active; }), entities.end());

    // Update all the entities.
    for (auto& e: entities)
    {
        if (!e.active) continue;

        e.old_pos.x = e.pos.x;
        e.old_pos.y = e.pos.y;

        // If the entity has a behavior then carry it out.
        if (e.behavior) e.behavior(e);
    }
}

INTERNAL void RenderEntities (std::vector<Entity>& entities)
{
    for (auto& e: entities)
    {
        if (!e.active) continue;

        // Smoothly lerp the entity from tile-to-tile to give a more fluid feel to the movement.
        float target_x = (float)e.pos.x*TILE_W;
        float target_y = (float)e.pos.y*TILE_H;
        e.draw.pos.x = Lerp(e.draw.pos.x, target_x, gApplication.delta_time*ENTITY_MOVE_SPEED);
        e.draw.pos.y = Lerp(e.draw.pos.y, target_y, gApplication.delta_time*ENTITY_MOVE_SPEED);

        // Smoothly lerp other values that have a current and a target for smoother visuals.
        e.draw.angle.current = Lerp(e.draw.angle.current, e.draw.angle.target, gApplication.delta_time*ENTITY_TURN_SPEED);
        e.draw.color.current = Lerp(e.draw.color.current, e.draw.color.target, gApplication.delta_time*ENTITY_COLOR_SPEED);

        // @Improve: Handle the direction you turn based on movement a bit better.
        // Handle rotating the entity slightly when they are moving for some nice visual flair.
        if (roundf(target_x) != roundf(e.draw.pos.x) || roundf(target_y) != roundf(e.draw.pos.y))
        {
            if ((roundf(target_x) > roundf(e.draw.pos.x)) || (roundf(target_y) > roundf(e.draw.pos.y))) e.draw.angle.target = ENTITY_TURN_ANGLE;
            else if ((roundf(target_x) < roundf(e.draw.pos.x)) || (roundf(target_y) < roundf(e.draw.pos.y))) e.draw.angle.target = -ENTITY_TURN_ANGLE;
        }
        else
        {
            e.draw.angle.target = 0.0f;
        }

        Vec2 center = { TILE_W/2, TILE_H/2 };
        DrawImage("entity", e.draw.pos.x,e.draw.pos.y, {1,1}, center, e.draw.angle.current, FLIP_NONE, e.draw.color.current, &e.draw.clip);

        // If it's a light source, draw a light.
        if (e.light)
        {
            float lx = e.draw.pos.x+(TILE_W/2);
            float ly = e.draw.pos.y+(TILE_H/2);
            DrawLight(lx,ly,e.light_radius);
        }
    }
}

//
// HELPERS
//

INTERNAL bool MoveEntity (Entity& e, int x, int y)
{
    // Don't move an entity out of the map bounds.
    if (x >= 0 && x < WORLD_W_IN_TILES && y >= 0 && y < WORLD_H_IN_TILES)
    {
        // Get the target tile and check if the move is valid.
        Tile* tile = MapGetTileAtPos(x,y);
        if (!tile || !tile->active || !tile->solid) // If there's no tile or it's not solid...
        {
            Entity* entity = MapGetEntityAtPos(x,y);
            if (!entity || !entity->active) // If there's no entity...
            {
                e.pos.x = x;
                e.pos.y = y;
                return true;
            }
        }
    }
    return false;
}

INTERNAL void DamageEntity (Entity& e, int damage)
{
    e.draw.color.current = SDLColorToColor({ 230, 72, 46, 255 });
    e.draw.angle.current = ENTITY_HIT_ANGLE;

    if (e.health != ENTITY_INVINCIBLE) e.health -= damage;

    // Handle visual particle effects.
    Rect particle_region = { e.draw.pos.x+(TILE_W/2),e.draw.pos.y+(TILE_H/2),0,0 };
    EntityBase& base = gEntities.at(e.type);
    if (e.active) for (auto& p: base.particle_hit) MapSpawnParticles(p, particle_region);

    if (!base.sound_hit.empty())
    {
        PlaySound(base.sound_hit.at(RandomRange(0,(int)base.sound_hit.size()-1)));
    }

    // If the health is below zero then kill the entity.
    if (e.active && e.health <= 0 && e.health != ENTITY_INVINCIBLE)
    {
        KillEntity(e);

        // Drops are handled here so if an entity is killed by calling KillEntity directly (which
        // is what the game does when night ends) the drops will not be given to the player on kill.
        if (!base.drops.empty())
        {
            for (auto& drop: base.drops)
            {
                int quantity = RandomRange(drop.min,drop.max);
                PlayerPickUpItem(drop.type, quantity);
            }
        }
    }
}

INTERNAL void KillEntity (Entity& e)
{
    // The entity is no longer active!
    e.active = false;

    // Handle visual particle effects.
    Rect particle_region = { e.draw.pos.x+(TILE_W/2),e.draw.pos.y+(TILE_H/2),0,0 };
    EntityBase& base = gEntities.at(e.type);
    for (auto& p: base.particle_dead) MapSpawnParticles(p, particle_region);
}

//
// BEHAVIORS
//

INTERNAL void Entity_BehaviorPlayer (Entity& e)
{
    // Handle Movement

    int movex = e.pos.x;
    int movey = e.pos.y;

    if (IsKeyDown(SDL_SCANCODE_W)) movey--;
    if (IsKeyDown(SDL_SCANCODE_D)) movex++;
    if (IsKeyDown(SDL_SCANCODE_S)) movey++;
    if (IsKeyDown(SDL_SCANCODE_A)) movex--;

    if (movex != e.pos.x || movey != e.pos.y) MoveEntity(e,movex,movey);

    // Handle Interactions (Attacking, Building)

    int targetx = e.pos.x;
    int targety = e.pos.y;

    if (IsKeyDown(SDL_SCANCODE_UP   )) targety--;
    if (IsKeyDown(SDL_SCANCODE_RIGHT)) targetx++;
    if (IsKeyDown(SDL_SCANCODE_DOWN )) targety++;
    if (IsKeyDown(SDL_SCANCODE_LEFT )) targetx--;

    if (targetx != e.pos.x || targety != e.pos.y)
    {
        Entity* o = MapGetEntityAtPos(targetx,targety);
        if (o)
        {
            // If there's an entity at our target location, then attack.
            e.draw.angle.current = ENTITY_TURN_ANGLE*3;
            std::string item_name = PlayerGetEquippedItemName();
            int damage = (item_name.empty()) ? 1 : GetItem(item_name).damage;
            DamageEntity(*o, damage);
        }
        else
        {
            // If there's a tile at our target location, then attack.
            Tile* t = MapGetTileAtPos(targetx,targety);
            if (t && t->active && t->hits > 0)
            {
                e.draw.angle.current = ENTITY_TURN_ANGLE*3;
                DamageTile(targetx,targety);
            }
            else
            {
                // Attempt to use whatever we have equipped.
                if (targetx != e.pos.x || targety != e.pos.y)
                {
                    std::string item_name = PlayerGetEquippedItemName();
                    if (!item_name.empty())
                    {
                        PlayerPlaceSelectedItem(targetx,targety); // Attempt to place.
                    }
                }
            }
        }
    }

    // Use Selected Item

    if (IsKeyDown(SDL_SCANCODE_SPACE))
    {
        std::string item_name = PlayerGetEquippedItemName();
        if (!item_name.empty())
        {
            auto& item = GetItem(item_name);
            if (item.category == "food") PlayerEatSelectedItem(); // Attempt to eat.
            else if (item.category == "magic") PlayerDrinkSelectedItem(); // Attempt to drink.
        }
    }
}

INTERNAL void Entity_BehaviorPassive (Entity& e)
{
    if (RandomRange(1,100) <= 25)
    {
        int targetx = e.pos.x;
        int targety = e.pos.y;

        Direction dir = (Direction)RandomRange(DIR_N,DIR_W);
        switch (dir)
        {
            case (DIR_N): targety--; break;
            case (DIR_E): targetx++; break;
            case (DIR_S): targety++; break;
            case (DIR_W): targetx--; break;
        }

        // Don't move to the location where an entity is.
        Entity* o = MapGetEntityAtPos(targetx,targety);
        if (!o) MoveEntity(e, targetx,targety);
    }
}

INTERNAL void Entity_BehaviorAggressive (Entity& e)
{
    if (RandomRange(1,100) <= 50)
    {
        // Track down the player if they're close enough otherwise just wander.
        Entity* p = MapGetFirstEntityOfType("player");
        if (p)
        {
            int distance = abs(Distance(p->pos.x,p->pos.y, e.pos.x,e.pos.y));
            if (distance <= 1) // If we're close enough to the player then maybe attack.
            {
                if (Random() % 2 == 0)
                {
                    e.draw.angle.current = ENTITY_TURN_ANGLE*3;
                    DamagePlayer(e.damage);
                }
            }
            else if (distance <= 30) // Otherwise chase the player.
            {
                auto path = FindPath({ e.pos.x,e.pos.y }, { p->pos.x, p->pos.y });
                if (!path.empty()) MoveEntity(e, path.back().x,path.back().y);
            }
            else // Or just wander around if not.
            {
                Entity_BehaviorPassive(e);
            }
        }
        else // Or just wander around if not.
        {
            Entity_BehaviorPassive(e);
        }
    }
}
