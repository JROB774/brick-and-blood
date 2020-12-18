#pragma once

// General-purpose particle systems and functionality.

struct Particle
{
    std::string type;

    Vec2 pos;

    float lifetime;

    float direction;
    float speed;
    float friction;
    float angle;
    float rotate_speed;

    // Drawing stuff.
    struct
    {
        SDL_Rect clip;

        struct
        {
            float current;
            float end;
        } scale;
        struct
        {
            Vec4 current;
            Vec4 end;
        } color;
    } draw;
};

INTERNAL void   InitParticles ();
INTERNAL void UpdateParticles (std::vector<Particle>& particles);
INTERNAL void RenderParticles (std::vector<Particle>& particles);
