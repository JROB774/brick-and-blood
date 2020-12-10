#ifndef ENTITY_HPP
#define ENTITY_HPP

enum EntityType
{
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_TOTAL
};

struct Entity
{
    EntityType type;

    struct { int x,y; } tile;

    // Drawing stuff.
    struct
    {
        Vec2 pos;
        float current_angle;
        float target_angle;
    } draw;
};

INTERNAL void SpawnEntity (EntityType type, int tilex, int tiley);

INTERNAL void UpdateEntities ();
INTERNAL void RenderEntities ();

//
// ENTITIES
//

typedef void(*EntityUpdate)(Entity& entity);

INTERNAL void EntityUpdatePlayer (Entity& e);

GLOBAL constexpr EntityUpdate ENTITY_UPDATE_PROCS[ENTITY_TYPE_TOTAL]
{
    EntityUpdatePlayer, // ENTITY_TYPE_PLAYER
};

#endif // ENTITY_HPP
