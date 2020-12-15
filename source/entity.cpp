GLOBAL constexpr float ENTITY_MOVE_SPEED  = 20.0f;
GLOBAL constexpr float ENTITY_COLOR_SPEED =  4.0f;
GLOBAL constexpr float ENTITY_TURN_SPEED  = 15.0f;
GLOBAL constexpr float ENTITY_TURN_ANGLE  = 10.0f;
GLOBAL constexpr float ENTITY_HIT_ANGLE   = 30.0f;

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

//
// HELPERS
//

INTERNAL void MoveEntity (Entity& e, int x, int y)
{
    // If the target is outside the map just allow the movement because there won't be tiles to do collision.
    if (x < 0 || y < 0 || x >= WORLD_W_IN_TILES || y >= WORLD_H_IN_TILES)
    {
        e.pos.x = x;
        e.pos.y = y;
        return;
    }

    // Get the current chunk the target is in.
    int cx = x / CHUNK_W;
    int cy = y / CHUNK_H;
    Chunk& chunk = gMap.chunks[cy][cx];

    // Get the target tile and check if the move is valid.
    int tx = x % CHUNK_W;
    int ty = y % CHUNK_H;
    Tile& tile = chunk.tiles[ty][tx];
    if (!tile.solid)
    {
        e.pos.x = x;
        e.pos.y = y;
    }
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
        Entity* o = MapGetEntityAtPos(targetx,targety);
        if (o) // If there's an entity at our target location, carry out the appropriate action.
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

    // Don't move to the location where an entity is.
    Entity* o = MapGetEntityAtPos(targetx,targety);
    if (!o) MoveEntity(e, targetx,targety);
}
