struct EntityBase
{
    std::string faction;
    int initiative;
    int health;
    EntityBehavior behavior;
    struct { int x,y; } image;
    Vec4 color;
};

std::map<std::string,EntityBase> gEntities;

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

        base.faction = data["faction"].String();
        base.initiative = data["initiative"].Int(INT_MAX);
        base.health = data["health"].Int(1);

        std::string behavior = data["behavior"].String("none");
        if (behavior != "none")
        {
            if (ENTITY_BEHAVIOR.count(behavior)) base.behavior = ENTITY_BEHAVIOR.at(behavior);
            else LOG_ERROR(ERR_MIN, "No known entity behavior: %s", behavior.c_str());
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

        gEntities.insert({ data.name, base });
    }
}

INTERNAL void UpdateEntities (std::vector<Entity>& entities)
{
    // Sort the entities based on initiative so they get updated in correct order.
    std::sort(entities.begin(), entities.end());

    // Update all the entities.
    for (auto& e: entities)
    {
        if (e.health <= 0) continue;

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
        if (e.health <= 0) continue;

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
    }
}

//
// HELPERS
//

INTERNAL bool MoveEntity (Entity& e, int x, int y)
{
    // Get the target tile and check if the move is valid.
    Tile* tile = MapGetTileAtPos(x,y);
    if (!tile || !tile->active || !tile->solid) // If there's no tile or it's not solid then move!
    {
        e.pos.x = x;
        e.pos.y = y;
        return true;
    }
    return false;
}

INTERNAL void DamageTile (Tile& t)
{
    if (t.hits != TILE_INDESTRUCTIBLE)
    {
        t.draw.angle.current = TILE_HIT_ANGLE;
        t.hits--;
        if (t.hits <= 0)
        {
            t.active = false;
            // @Incomplete: ...
        }
    }
}

INTERNAL void DamageEntity (Entity& e)
{
    e.draw.color.current = SDLColorToColor({ 230, 72, 46, 255 });
    e.draw.angle.current = ENTITY_HIT_ANGLE;
    e.health--;
    if (e.health <= 0)
    {
        // @Incomplete: ...
    }
}

//
// BEHAVIORS
//

INTERNAL void Entity_BehaviorPlayer (Entity& e)
{
    int targetx = e.pos.x;
    int targety = e.pos.y;

    if (IsKeyDown(SDL_SCANCODE_W)) targety--;
    if (IsKeyDown(SDL_SCANCODE_D)) targetx++;
    if (IsKeyDown(SDL_SCANCODE_S)) targety++;
    if (IsKeyDown(SDL_SCANCODE_A)) targetx--;

    // If our target is not our tile handle attacking, moving, etc.
    if (targetx != e.pos.x || targety != e.pos.y)
    {
        Entity* o = MapGetEntityAtPos(targetx,targety);
        if (o) // If there's an entity at our target location, carry out the appropriate action.
        {
            e.draw.angle.current = ENTITY_TURN_ANGLE;
            DamageEntity(*o);
        }
        else // Othewise handle the case of tiles.
        {
            // If there is a tile in the space the player wants to move to we attack it, otherwise we move.
            Tile* t = MapGetTileAtPos(targetx,targety);
            if (t && t->active && t->hits > 0)
            {
                e.draw.angle.current = ENTITY_TURN_ANGLE;
                DamageTile(*t);
            }
            else
            {
                MoveEntity(e, targetx,targety);
            }
        }
    }
}

INTERNAL void Entity_BehaviorWander (Entity& e)
{
    if (Random() % 2 == 0) // 50/50 chance of moving.
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
