GLOBAL constexpr float ENTITY_MOVE_SPEED = 20.0f;
GLOBAL constexpr float ENTITY_COLOR_SPEED = 2.5f;
GLOBAL constexpr float ENTITY_TURN_SPEED = 15.0f;

struct EntityBase
{
    EntitySpawn spawn;
    EntityUpdate update;
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
    e.tile.x = tilex;
    e.tile.y = tiley;
    e.draw.pos.x = (float)(tilex*16);
    e.draw.pos.x = (float)(tiley*16);
    e.draw.angle.current = 0.0f;
    e.draw.angle.target = 0.0f;
    e.draw.color.current = base.color;
    e.draw.color.target = base.color;

    // If the entity has extra spawn functionality, do it.
    if (base.spawn) base.spawn(e);
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

        std::string spawn_name = data["spawn"].String("");
        if (ENTITY_SPAWN_PROC.count(spawn_name)) base.spawn = ENTITY_SPAWN_PROC.at(spawn_name);
        std::string update_name = data["update"].String("");
        if (ENTITY_UPDATE_PROC.count(update_name)) base.update = ENTITY_UPDATE_PROC.at(update_name);

        if (data.Contains("color"))
        {
            base.color.r = (float)data["color"][0].Number(1.0);
            base.color.g = (float)data["color"][0].Number(1.0);
            base.color.b = (float)data["color"][0].Number(1.0);
            base.color.a = (float)data["color"][0].Number(1.0);
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
    for (auto& e: gEntitySystem.entities)
    {
        // Smoothly lerp the player from tile-to-tile to give a more fluid feel to the movement.
        float target_x = (float)e.tile.x*16;
        float target_y = (float)e.tile.y*16;
        e.draw.pos.x = Lerp(e.draw.pos.x, target_x, gApplication.delta_time*ENTITY_MOVE_SPEED);
        e.draw.pos.y = Lerp(e.draw.pos.y, target_y, gApplication.delta_time*ENTITY_MOVE_SPEED);

        // Smoothly lerp other values that have a current and a target for smoother visuals.
        e.draw.angle.current = Lerp(e.draw.angle.current, e.draw.angle.target, gApplication.delta_time*ENTITY_TURN_SPEED);
        e.draw.color.current = Lerp(e.draw.color.current, e.draw.color.target, gApplication.delta_time*ENTITY_COLOR_SPEED);

        // If the entity has extra update functionality, do it.
        EntityBase& base = gEntitySystem.entity_base.at(e.base_type);
        if (base.update) base.update(e);
    }
}

INTERNAL void RenderEntities ()
{
    for (auto& e: gEntitySystem.entities)
    {
        DrawImage("entity", e.draw.pos.x,e.draw.pos.y, e.draw.angle.current, FLIP_NONE, e.draw.color.current);
    }
}

//
// Spawn and Update Behaviours
//

GLOBAL constexpr float PLAYER_TURN_ANGLE = 10.0f;

INTERNAL void Entity_PlayerSpawn (Entity& e)
{
    // Nothing...
}

INTERNAL void Entity_PlayerUpdate (Entity& e)
{
    // Handle actual tile-to-tile movement.
    if (IsKeyPressed(SDL_SCANCODE_A)) e.tile.x--;
    if (IsKeyPressed(SDL_SCANCODE_D)) e.tile.x++;
    if (IsKeyPressed(SDL_SCANCODE_W)) e.tile.y--;
    if (IsKeyPressed(SDL_SCANCODE_S)) e.tile.y++;

    float target_x = (float)e.tile.x*16;
    float target_y = (float)e.tile.y*16;

    // @Improve: Handle the direction you turn based on movement a bit better.
    // Handle rotating the player slightly when they are moving for some nice visual flair.
    if (roundf(target_x) != roundf(e.draw.pos.x) || roundf(target_y) != roundf(e.draw.pos.y))
    {
        if ((roundf(target_x) > roundf(e.draw.pos.x)) || (roundf(target_y) > roundf(e.draw.pos.y))) e.draw.angle.target = PLAYER_TURN_ANGLE;
        else if ((roundf(target_x) < roundf(e.draw.pos.x)) || (roundf(target_y) < roundf(e.draw.pos.y))) e.draw.angle.target = -PLAYER_TURN_ANGLE;
    }
    else
    {
        e.draw.angle.target = 0.0f;
    }
}
