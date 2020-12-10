GLOBAL struct EntitySystem
{
    std::vector<Entity> entities;

} gEntitySystem;

INTERNAL void SpawnEntit (EntityType type, int tilex, int tiley)
{
    Entity e;
    e.type = type;
    e.tile.x = tilex;
    e.tile.y = tiley;
    e.draw.pos.x = (float)(tilex*16);
    e.draw.pos.x = (float)(tiley*16);
    e.draw.current_angle = 0.0f;
    e.draw.target_angle  = 0.0f;
    gEntitySystem.entities.push_back(e);
}

INTERNAL void UpdateEntities ()
{
    for (auto& e: gEntitySystem.entities)
    {
        ENTITY_UPDATE_PROCS[e.type](e);
    }
}

INTERNAL void RenderEntities ()
{
    for (auto& e: gEntitySystem.entities)
    {
        DrawImage("entity", e.draw.pos.x,e.draw.pos.y, e.draw.current_angle);
    }
}

//
// ENTITY_TYPE_PLAYER
//

GLOBAL constexpr float PLAYER_MOVE_SPEED = 20.0f;
GLOBAL constexpr float PLAYER_TURN_ANGLE = 10.0f;
GLOBAL constexpr float PLAYER_TURN_SPEED = 15.0f;

INTERNAL void EntityUpdatePlayer (Entity& e)
{
    // Handle actual tile-to-tile movement.
    if (IsKeyPressed(SDL_SCANCODE_A)) e.tile.x--;
    if (IsKeyPressed(SDL_SCANCODE_D)) e.tile.x++;
    if (IsKeyPressed(SDL_SCANCODE_W)) e.tile.y--;
    if (IsKeyPressed(SDL_SCANCODE_S)) e.tile.y++;

    float target_x = (float)e.tile.x*16;
    float target_y = (float)e.tile.y*16;

    // Smoothly lerp the player from tile-to-tile to give a more fluid feel to the movement and visuals.
    e.draw.pos.x = Lerp(e.draw.pos.x, target_x, gApplication.delta_time*PLAYER_MOVE_SPEED);
    e.draw.pos.y = Lerp(e.draw.pos.y, target_y, gApplication.delta_time*PLAYER_MOVE_SPEED);

    // Handle rotating the player slightly when they are moving for some nice visual flair.
    if (roundf(target_x) != roundf(e.draw.pos.x))
    {
        if (roundf(target_x) > roundf(e.draw.pos.x)) e.draw.target_angle = PLAYER_TURN_ANGLE;
        else if ((roundf(target_x) < roundf(e.draw.pos.x))) e.draw.target_angle = -PLAYER_TURN_ANGLE;
    }
    else
    {
        e.draw.target_angle = 0.0f;
    }

    e.draw.current_angle = Lerp(e.draw.current_angle, e.draw.target_angle, gApplication.delta_time*PLAYER_TURN_SPEED);
}
