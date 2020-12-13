INTERNAL void MapSpawnEntity (std::string type, int x, int y)
{
    // If the specified base type can't be found then we don't create an entity.
    if (!gEntitySystem.entity_base.count(type))
    {
        LOG_ERROR(ERR_MIN, "Could not create entity of unknown type: %s", type.c_str());
        return;
    }
    EntityBase& base = gEntitySystem.entity_base.at(type);

    // Create the actual entity and add it to the manager.
    gMap.entities.push_back(Entity());
    Entity& e = gMap.entities.back();

    e.type = type;
    e.faction = base.faction;
    e.initiative = base.initiative;
    e.behavior = base.behavior;
    e.health = base.health;
    e.pos.x = x;
    e.pos.y = y;
    e.old_pos.x = x;
    e.old_pos.y = y;
    e.draw.pos.x = (float)(x*TILE_W);
    e.draw.pos.y = (float)(y*TILE_H);
    e.draw.clip.x = base.image.x*TILE_W;
    e.draw.clip.y = base.image.y*TILE_H;
    e.draw.clip.w = TILE_W;
    e.draw.clip.h = TILE_H;
    e.draw.angle.current = 0.0f;
    e.draw.angle.target = 0.0f;
    e.draw.color.current = base.color;
    e.draw.color.target = base.color;
}

INTERNAL Entity* MapGetEntityAtPos (int x, int y)
{
    for (auto& e: gMap.entities)
    {
        if (e.pos.x == x && e.pos.y == y)
        {
            if (e.health > 0) return &e;
            break;
        }
    }
    return NULL;
}

INTERNAL void InitMap ()
{
    // Nothing...
}

INTERNAL void QuitMap ()
{
    // Nothing...
}

INTERNAL void UpdateMap ()
{
    // Update entities.
    if (gPlayer.update)
    {
        // Sort the entities based on initiative so they get updated in correct order.
        std::sort(gMap.entities.begin(), gMap.entities.end());

        // Update all the entities.
        for (auto& e: gMap.entities)
        {
            if (e.health <= 0) continue;

            e.old_pos.x = e.pos.x;
            e.old_pos.y = e.pos.y;

            // If the entity has a behavior then carry it out.
            if (e.behavior) e.behavior(e);
        }
    }
}

INTERNAL void RenderMap ()
{
    // Render entities.
    for (auto& e: gMap.entities)
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
        DrawImage("entity", e.draw.pos.x,e.draw.pos.y, center, e.draw.angle.current, FLIP_NONE, e.draw.color.current, &e.draw.clip);
    }
}
