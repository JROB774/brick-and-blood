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

struct ParticleBase
{
    std::vector<SDL_Rect> clips;
    float lifetime_min, lifetime_max;
    float speed_min, speed_max;
    float friction_min, friction_max;
    float angle_min, angle_max;
    float rotate_speed_min, rotate_speed_max;
    float start_scale_min, start_scale_max;
    float end_scale_min, end_scale_max;
    Vec4  start_color;
    Vec4  end_color;
    int   spawn_count_min, spawn_count_max;
    float spawn_angle_min, spawn_angle_max;
};

INTERNAL void   InitParticles ();
INTERNAL void UpdateParticles (std::vector<Particle>& particles);
INTERNAL void RenderParticles (std::vector<Particle>& particles);
