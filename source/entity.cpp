GLOBAL constexpr float ENTITY_MOVE_SPEED  = 20.0f;
GLOBAL constexpr float ENTITY_COLOR_SPEED =  4.0f;
GLOBAL constexpr float ENTITY_TURN_SPEED  = 15.0f;
GLOBAL constexpr float ENTITY_TURN_ANGLE  = 10.0f;
GLOBAL constexpr float ENTITY_HIT_ANGLE   = 30.0f;

struct EntityBase
{
    int initiative;
    int health;
    EntityBehavior behavior;
    struct { int x,y; } image;
    Vec4 color;
};

GLOBAL struct EntitySystem
{
    std::map<std::string,EntityBase> entity_base;
    std::vector<Entity> entities;

} gEntitySystem;

INTERNAL void SpawnEntity (std::string base_type, int tilex, int tiley)
{
    // If the specified base type can't be found then we don't create an entity.
    if (!gEntitySystem.entity_base.count(base_type))
    {
        LOG_ERROR(ERR_MIN, "Could not create entity of unknown type: %s", base_type.c_str());
        return;
    }
    EntityBase& base = gEntitySystem.entity_base.at(base_type);

    // Create the actual entity and add it to the manager.
    gEntitySystem.entities.push_back(Entity());
    Entity& e = gEntitySystem.entities.back();

    e.base_type = base_type;
    e.pos.x = tilex;
    e.pos.y = tiley;
    e.old_pos.x = tilex;
    e.old_pos.y = tiley;
    e.initiative = base.initiative;
    e.behavior = base.behavior;
    e.health = base.health;
    e.draw.pos.x = (float)(tilex*TILE_W);
    e.draw.pos.y = (float)(tiley*TILE_H);
    e.draw.clip.x = base.image.x*TILE_W;
    e.draw.clip.y = base.image.y*TILE_H;
    e.draw.clip.w = TILE_W;
    e.draw.clip.h = TILE_H;
    e.draw.angle.current = 0.0f;
    e.draw.angle.target = 0.0f;
    e.draw.color.current = base.color;
    e.draw.color.target = base.color;
}

INTERNAL Entity* GetEntityAtPos (int x, int y)
{
    for (auto& e: gEntitySystem.entities)
    {
        if (e.pos.x == x && e.pos.y == y) return &e;
    }
    return NULL;
}

INTERNAL void InitEntities ()
{
    GonObject& gon = *GetAsset<AssetData>("entities");
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Entity data not formatted as object!");
        return;
    }

    // Go through each base entity type and cache the data.
    for (auto& data: gon.children_array)
    {
        EntityBase base = {};

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

        gEntitySystem.entity_base.insert({ data.name, base });
    }
}

INTERNAL void UpdateEntities ()
{
    // Sort the entities based on initiative so they get updated in correct order.
    std::sort(gEntitySystem.entities.begin(), gEntitySystem.entities.end());

    // Update all the entities.
    for (auto& e: gEntitySystem.entities)
    {
        if (e.health <= 0) continue;

        e.old_pos.x = e.pos.x;
        e.old_pos.y = e.pos.y;

        // If the entity has a behavior then carry it out.
        if (e.behavior) e.behavior(e);
    }
}

INTERNAL void RenderEntities ()
{
    for (auto& e: gEntitySystem.entities)
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

//
// HELPERS
//

INTERNAL void MoveEntity (Entity& e, int x, int y)
{
    // @Incomplete: Handle tile collision...
    e.pos.x = x;
    e.pos.y = y;
}

INTERNAL void DamageEntity (Entity& e)
{
    e.draw.color.current = SDLColorToColor({ 230, 72, 46, 255 });
    e.draw.angle.current = ENTITY_HIT_ANGLE;
    e.health--;
}

//
// BEHAVIORS
//

INTERNAL void Entity_BehaviorPlayer (Entity& e)
{
    int targetx = e.pos.x;
    int targety = e.pos.y;

    if (IsKeyPressed(SDL_SCANCODE_W)) targety--;
    if (IsKeyPressed(SDL_SCANCODE_D)) targetx++;
    if (IsKeyPressed(SDL_SCANCODE_S)) targety++;
    if (IsKeyPressed(SDL_SCANCODE_A)) targetx--;

    // If our target is not our tile handle attacking, moving, etc.
    if (targetx != e.pos.x || targety != e.pos.y)
    {
        Entity* o = GetEntityAtPos(targetx,targety);
        if (o && o->health > 0) // If there's an entity at our target location, carry out the appropriate action.
        {
            e.draw.angle.current = ENTITY_TURN_ANGLE;
            DamageEntity(*o);
        }
        else // Otherwise move to that tile.
        {
            MoveEntity(e, targetx,targety);
        }
    }
}

INTERNAL void Entity_BehaviorWander (Entity& e)
{
    int targetx = e.pos.x;
    int targety = e.pos.y;

    Direction dir = (Direction)RandomRange(DIR_N,DIR_S);
    switch (dir)
    {
        case (DIR_N): targety--; break;
        case (DIR_E): targetx++; break;
        case (DIR_S): targety++; break;
        case (DIR_W): targetx--; break;
    }

    // Don't move to the location where the player is.
    Entity* o = GetEntityAtPos(targetx,targety);
    if (!o || o->health <= 0 || o->base_type != "player")
    {
        MoveEntity(e, targetx,targety);
    }
}
