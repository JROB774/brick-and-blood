#ifndef ENTITY_HPP
#define ENTITY_HPP

// List of all the different entity types.

enum EntityType
{
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_TOTAL
};

// General-purpose entity systems and functionality.

struct Entity
{
    EntityType type;

    struct { int x,y; } tile;

    // Drawing stuff.
    struct
    {
        Vec2 pos;

        struct
        {
            float current;
            float target;
        }  angle;
        struct
        {
            Vec4 current;
            Vec4 target;
        } color;
    } draw;
};

INTERNAL void SpawnEntity (EntityType type, int tilex, int tiley);

INTERNAL void UpdateEntities ();
INTERNAL void RenderEntities ();

// Spawn and update functions for the different entity types.

INTERNAL void Entity_PlayerSpawn  (Entity& e);
INTERNAL void Entity_PlayerUpdate (Entity& e);

// Base information for each type of entity.

typedef void(*EntitySpawn)(Entity& e);
typedef void(*EntityUpdate)(Entity& e);

struct EntityBase
{
    EntitySpawn spawn;
    EntityUpdate update;
    Vec4 base_color;
};

GLOBAL constexpr EntityBase ENTITY_BASE_LIST[ENTITY_TYPE_TOTAL]
{
    { Entity_PlayerSpawn, Entity_PlayerUpdate, { 0.81f,0.77f,0.72f,1.0f } } // ENTITY_TYPE_PLAYER
};

#endif // ENTITY_HPP
