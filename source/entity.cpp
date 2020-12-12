GLOBAL constexpr float ENTITY_MOVE_SPEED = 20.0f;
GLOBAL constexpr float ENTITY_COLOR_SPEED = 2.5f;
GLOBAL constexpr float ENTITY_TURN_SPEED = 15.0f;
GLOBAL constexpr float ENTITY_TURN_ANGLE = 10.0f;

struct EntityBase
{
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
    e.timer = 0.0f;
    e.behavior = base.behavior;
    e.draw.pos.x = (float)(tilex*16);
    e.draw.pos.y = (float)(tiley*16);
    e.draw.clip.x = base.image.x*16;
    e.draw.clip.y = base.image.y*16;
    e.draw.clip.w = 16;
    e.draw.clip.h = 16;
    e.draw.angle.current = 0.0f;
    e.draw.angle.target = 0.0f;
    e.draw.color.current = base.color;
    e.draw.color.target = base.color;
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

        std::string behavior = data["behavior"].String("");
        if (ENTITY_BEHAVIOR.count(behavior)) base.behavior = ENTITY_BEHAVIOR.at(behavior);

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
    for (auto& e: gEntitySystem.entities)
    {
        // If the entity has a behavior then carry it out.
        if (e.behavior) e.behavior(e);

        // Smoothly lerp the entity from tile-to-tile to give a more fluid feel to the movement.
        float target_x = (float)e.pos.x*16;
        float target_y = (float)e.pos.y*16;
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
    }
}

INTERNAL void RenderEntities ()
{
    for (auto& e: gEntitySystem.entities)
    {
        Vec2 center = { (float)e.draw.clip.x+(16/2), (float)e.draw.clip.y+(16/2) };
        DrawImage("entity", e.draw.pos.x,e.draw.pos.y, center, e.draw.angle.current, FLIP_NONE, e.draw.color.current, &e.draw.clip);
    }
}

//
// Behaviors
//

INTERNAL void Entity_BehaviorPlayer (Entity& e)
{
    // Handle actual tile-to-tile movement.
    if (IsKeyPressed(SDL_SCANCODE_A)) e.pos.x--;
    if (IsKeyPressed(SDL_SCANCODE_D)) e.pos.x++;
    if (IsKeyPressed(SDL_SCANCODE_W)) e.pos.y--;
    if (IsKeyPressed(SDL_SCANCODE_S)) e.pos.y++;
}

INTERNAL void Entity_BehaviorWander (Entity& e)
{
    e.timer += gApplication.delta_time;
    if (e.timer >= 0.75f)
    {
        e.timer -= 0.75f;

        int dir = RandomRange(0,3);
        switch (dir)
        {
            case (0): e.pos.x--;
            case (1): e.pos.x++;
            case (2): e.pos.y--;
            case (3): e.pos.y++;
        }
    }
}
