#pragma once

// General-purpose entity systems and functionality.

enum Direction { DIR_N, DIR_E, DIR_S, DIR_W };

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

// Interface
INTERNAL void    SpawnEntity    (std::string base_type, int tilex, int tiley);
INTERNAL Entity* GetEntityAtPos (int x, int y); // NULL if no entity found!

INTERNAL void   InitEntities ();
INTERNAL void UpdateEntities ();
INTERNAL void RenderEntities ();

//
// BEHAVIORS
//

INTERNAL void Entity_BehaviorPlayer (Entity& e);
INTERNAL void Entity_BehaviorWander (Entity& e);

GLOBAL const std::map<std::string,EntityBehavior> ENTITY_BEHAVIOR
{
    { "BehaviorPlayer", Entity_BehaviorPlayer },
    { "BehaviorWander", Entity_BehaviorWander },
};
