#ifndef PARTICLE_HPP
#define PARTICLE_HPP

enum ParticleType
{
    PARTICLE_TYPE_TEST,
    PARTICLE_TYPE_SPEC,
    PARTICLE_TYPE_PUFF,
    PARTICLE_TYPE_BASH,
    PARTICLE_TYPE_EXPLODE1,
    PARTICLE_TYPE_SMOKE1,
    PARTICLE_TYPE_SMOKE2,
    PARTICLE_TYPE_SBONE,
    PARTICLE_TYPE_LBONE,
    PARTICLE_TYPE_SBREAK,
    PARTICLE_TYPE_MBREAK,
    PARTICLE_TYPE_LBREAK,
    PARTICLE_TYPE_PUFF_D,
    PARTICLE_TYPE_TOTAL
};

struct Particle
{
    ParticleType type;
    Vec2 pos,vel;
    float angle; // In radians.
    Animation anim;
    Color color;
    bool dead;
    bool visible;

    // -1 can be used to make its life infinite. Which allows for the programmer
    // to specify a different method of deletion in the particle's update e.g.
    // after coming to a complete stop, travelling off-screen, etc.
    float lifetime;

    // Extra variables, do what you want with them if you need to store extra data.
    // Can be accessed as either a signed/unsigned integer or a floating-point.
    union { unsigned int u; int i; float f; } extra0, extra1, extra2, extra3;
};

INTERNAL void InitParticleSystem ();
INTERNAL void QuitParticleSystem ();

INTERNAL void CreateParticles (ParticleType type, int minx, int miny, int maxx, int maxy, int min_count, int max_count, float lifetime_multiplier = 1.0f);
INTERNAL void CreateParticles (ParticleType type, int minx, int miny, int maxx, int maxy, int     count,                float lifetime_multiplier = 1.0f);

INTERNAL void UpdateParticles ();
INTERNAL void DrawParticles   ();
INTERNAL void ClearParticles  ();

// Create and update functions for the different particle types.

// ...

// Base information for each type of particle.

typedef void(*ParticleCreate)(Particle& particle);
typedef void(*ParticleUpdate)(Particle& particle, float dt);

struct ParticleBase
{
    ParticleCreate create;
    ParticleUpdate update;
    std::vector<std::string> anims;
    float min_lifetime;
    float max_lifetime;
};

GLOBAL const ParticleBase PARTICLE_BASE[PARTICLE_TYPE_TOTAL]
{
// ...
};

#endif /* PARTICLE_HPP */
