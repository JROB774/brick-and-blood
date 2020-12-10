#ifndef ENTITY_HPP
#define ENTITY_HPP

// General-purpose entity systems and functionality.

struct Entity
{
    std::string base_type;

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

INTERNAL void SpawnEntity (std::string base_type, int tilex, int tiley);

INTERNAL void   InitEntities ();
INTERNAL void UpdateEntities ();
INTERNAL void RenderEntities ();

// Different spawn and update behaviours that entity types can use.

typedef void(*EntitySpawn)(Entity& e);
typedef void(*EntityUpdate)(Entity& e);

INTERNAL void Entity_PlayerSpawn  (Entity& e);
INTERNAL void Entity_PlayerUpdate (Entity& e);

GLOBAL const std::map<std::string,EntitySpawn> ENTITY_SPAWN_PROC
{
    { "PlayerSpawn", Entity_PlayerSpawn },
};
GLOBAL const std::map<std::string,EntityUpdate> ENTITY_UPDATE_PROC
{
    { "PlayerUpdate", Entity_PlayerUpdate },
};

#endif // ENTITY_HPP
