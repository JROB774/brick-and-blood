#pragma once

// General-purpose entity systems and functionality.

struct Entity;

typedef void(*EntityBehavior)(Entity& e);

struct Entity
{
    std::string base_type;

    struct { int x,y; } pos, old_pos;

    int initiative; // Controls what entity acts first.

    EntityBehavior behavior;

    // Drawing stuff.
    struct
    {
        Vec2 pos;
        SDL_Rect clip;

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

    // For sorting entities.
    bool operator < (const Entity& e)
    {
        return (initiative < e.initiative);
    }
};

INTERNAL void SpawnEntity (std::string base_type, int tilex, int tiley);

INTERNAL void   InitEntities ();
INTERNAL void UpdateEntities ();
INTERNAL void RenderEntities ();

// Different spawn and update behaviors that entity types can use.

INTERNAL void Entity_BehaviorPlayer (Entity& e);
INTERNAL void Entity_BehaviorWander (Entity& e);

GLOBAL const std::map<std::string,EntityBehavior> ENTITY_BEHAVIOR
{
    { "BehaviorPlayer", Entity_BehaviorPlayer },
    { "BehaviorWander", Entity_BehaviorWander },
};
