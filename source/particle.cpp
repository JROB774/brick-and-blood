GLOBAL struct ParticleSystem
{
    std::vector<Particle> particles;
    float clear_timer;

} gParticleSystem;

INTERNAL void InitParticleSystem ()
{
    gParticleSystem.clear_timer = 0.0f;
}

INTERNAL void QuitParticleSystem ()
{
    gParticleSystem.particles.clear();
}

INTERNAL void CreateParticles (ParticleType type, int minx, int miny, int maxx, int maxy, int min_count, int max_count, float lifetime_multiplier)
{
    int count = RandomRange(min_count, max_count);
    for (int i=0; i<count; ++i)
    {
        gParticleSystem.particles.push_back(Particle());

        Particle& particle = gParticleSystem.particles.back();
        const ParticleBase& base = PARTICLE_BASE[type];

        ASSERT(base.create);
        ASSERT(base.update);

        particle.type = type;
        particle.vel = { 0,0 };
        CreateAnimation(particle.anim, base.anims[RandomRange(0, (int)base.anims.size()-1)]);
        particle.color = { 1,1,1,1 };
        particle.dead = false;
        particle.visible = true;
        particle.lifetime = RandomFloatRange(base.min_lifetime, base.max_lifetime) * lifetime_multiplier;

        int fw = GetAnimationFrameWidth(particle.anim);
        int fh = GetAnimationFrameHeight(particle.anim);

        particle.pos.x = (float)RandomRange(minx-(fw/2), maxx-(fw/2));
        particle.pos.y = (float)RandomRange(miny-(fh/2), maxy-(fh/2));

        base.create(particle);
    }
}

INTERNAL void CreateParticles (ParticleType type, int minx, int miny, int maxx, int maxy, int count, float lifetime_multiplier)
{
    CreateParticles(type, minx, miny, maxx, maxy, count, count, lifetime_multiplier);
}

INTERNAL void UpdateParticles ()
{
    for (auto& particle: gParticleSystem.particles)
    {
        if (!particle.dead)
        {
            PARTICLE_BASE[particle.type].update(particle, gApplication.delta_time);
            // Update certain default particle values so they don't have to be repeated in each individual particle update.
            particle.pos.x += particle.vel.x * gApplication.delta_time;
            particle.pos.y += particle.vel.y * gApplication.delta_time;
            if (particle.lifetime > 0.0f) // Particles with negative lifetime don't use it.
            {
                particle.lifetime -= gApplication.delta_time;
                if (particle.lifetime <= 0.0f)
                {
                    particle.dead = true;
                }
            }
        }
    }

    // Every few seconds we remove dead particles from the array to keep memory usage low.
    gParticleSystem.clear_timer += gApplication.delta_time;
    if (gParticleSystem.clear_timer >= 5.0f)
    {
        gParticleSystem.particles.erase(std::remove_if(
            gParticleSystem.particles.begin(), gParticleSystem.particles.end(),
            [](const Particle& p) { return p.dead; }),
            gParticleSystem.particles.end()
        );
    }
}

INTERNAL void DrawParticles ()
{
    for (auto& particle: gParticleSystem.particles)
    {
        if (particle.visible && !particle.dead)
        {
            UpdateAnimation(particle.anim, gApplication.delta_time);
            DrawImage("particle", particle.pos.x, particle.pos.y, {0,0}, particle.angle, FLIP_NONE, particle.color, GetAnimationClip(particle.anim));
        }
    }
}

INTERNAL void ClearParticles ()
{
    gParticleSystem.particles.clear();
}
