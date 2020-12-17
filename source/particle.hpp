#pragma once

// General-purpose particle systems and functionality.

struct Particle
{
    std::string type;

    Vec2 pos;
    Vec2 vel;

    float lifetime;

    // Drawing stuff.
    struct
    {
        SDL_Rect clip;

        Vec4 color;

        float angle;
        float scale;
    } draw;

    float angle;
    float scale;
    Vec4 color;
};

INTERNAL void   InitParticles ();
INTERNAL void UpdateParticles (std::vector<Particle>& particles);
INTERNAL void RenderParticles (std::vector<Particle>& particles);
