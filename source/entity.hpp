#pragma once

// General-purpose entity systems and functionality.

GLOBAL constexpr float ENTITY_MOVE_SPEED  = 20.0f;
GLOBAL constexpr float ENTITY_COLOR_SPEED =  4.0f;
GLOBAL constexpr float ENTITY_TURN_SPEED  = 15.0f;
GLOBAL constexpr float ENTITY_TURN_ANGLE  = 10.0f;
GLOBAL constexpr float ENTITY_HIT_ANGLE   = 50.0f;

GLOBAL constexpr int ENTITY_INVINCIBLE = -9999;

enum Direction { DIR_N, DIR_E, DIR_S, DIR_W };

struct Entity;

typedef void(*EntityBehavior)(Entity& e);

struct Entity
{
    std::string type;

    std::string faction;
    std::string state;

    int initiative; // Controls what entity acts first.
    int health;
    int damage;

    bool light;
    float light_radius;

    std::string behavior_type;
    EntityBehavior behavior;

    struct { int x,y; } pos, old_pos;

    bool active;

    // Drawing stuff.
    struct
    {
        Vec2 pos;
        SDL_Rect clip;

        struct
        {
            float current;
            float target;
        } angle;
        struct
        {
            Vec4 current;
            Vec4 target;
        } color;
    } draw;

    // For sorting entities.
    bool operator< (const Entity& e) const
    {
        return (initiative < e.initiative);
    }
};

struct EntityDrops
{
    std::string type;
    int min, max;
};

struct EntityBase
{
    std::string pretty_name;
    std::string faction;
    std::string state;
    int initiative;
    int health;
    int damage;
    bool light;
    float light_radius;
    std::string behavior_type;
    EntityBehavior behavior;
    struct { int x,y; } image;
    Vec4 color;
    std::vector<std::string> particle_hit;
    std::vector<std::string> particle_dead;
    std::vector<std::string> sound_hit;
    std::vector<EntityDrops> drops;
};

INTERNAL std::vector<std::string> GetAllEntityTypesOfFaction (std::string faction);

INTERNAL void   InitEntities ();
INTERNAL void UpdateEntities (std::vector<Entity>& entities);
INTERNAL void RenderEntities (std::vector<Entity>& entities);

//
// HELPERS
//

INTERNAL bool MoveEntity (Entity& e, int x, int y);
INTERNAL void DamageEntity (Entity& e, int damage);

//
// BEHAVIORS
//

INTERNAL void Entity_BehaviorPlayer     (Entity& e);
INTERNAL void Entity_BehaviorPassive    (Entity& e);
INTERNAL void Entity_BehaviorAggressive (Entity& e);
INTERNAL void Entity_BehaviorAggroOnHit (Entity& e);

GLOBAL const std::map<std::string,EntityBehavior> ENTITY_BEHAVIOR
{
    { "BehaviorPlayer",     Entity_BehaviorPlayer     },
    { "BehaviorPassive",    Entity_BehaviorPassive    },
    { "BehaviorAggressive", Entity_BehaviorAggressive },
    { "BehaviorAggroOnHit", Entity_BehaviorAggroOnHit }
};
