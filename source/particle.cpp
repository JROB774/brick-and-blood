struct ParticleBase
{
    struct { int x,y; } image_start;
    int   image_count;
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

std::map<std::string,ParticleBase> gParticles;

INTERNAL void InitParticles ()
{
    gParticles.clear();

    GonObject& gon = *GetAsset<AssetData>("particles");
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Particle data not formatted as object!");
        return;
    }

    // Go through each base particle type and cache the data.
    for (auto& data: gon.children_array)
    {
        // Warn of particles with duplicate names.
        if (gParticles.count(data.name))
        {
            LOG_ERROR(ERR_MIN, "Warning duplicate particle with the name %s!", data.name.c_str());
        }

        ParticleBase base = {};

        base.image_start.x = data["image_start"][0].Int(0);
        base.image_start.y = data["image_start"][1].Int(0);

        base.image_count = data["image_count"].Int(1);

        if (data["lifetime"].type == GonObject::FieldType::ARRAY)
        {
            base.lifetime_min = (float)data["lifetime"][0].Number();
            base.lifetime_max = (float)data["lifetime"][1].Number();
        }
        else
        {
            base.lifetime_min = (float)data["lifetime"].Number();
            base.lifetime_max = (float)data["lifetime"].Number();
        }
        if (data["speed"].type == GonObject::FieldType::ARRAY)
        {
            base.speed_min = (float)data["speed"][0].Number();
            base.speed_max = (float)data["speed"][1].Number();
        }
        else
        {
            base.speed_min = (float)data["speed"].Number();
            base.speed_max = (float)data["speed"].Number();
        }
        if (data["friction"].type == GonObject::FieldType::ARRAY)
        {
            base.friction_min = (float)data["friction"][0].Number();
            base.friction_max = (float)data["friction"][1].Number();
        }
        else
        {
            base.friction_min = (float)data["friction"].Number();
            base.friction_max = (float)data["friction"].Number();
        }
        if (data["angle"].type == GonObject::FieldType::ARRAY)
        {
            base.angle_min = (float)data["angle"][0].Number();
            base.angle_max = (float)data["angle"][1].Number();
        }
        else
        {
            base.angle_min = (float)data["angle"].Number();
            base.angle_max = (float)data["angle"].Number();
        }
        if (data["rotate_speed"].type == GonObject::FieldType::ARRAY)
        {
            base.rotate_speed_min = (float)data["rotate_speed"][0].Number();
            base.rotate_speed_max = (float)data["rotate_speed"][1].Number();
        }
        else
        {
            base.rotate_speed_min = (float)data["rotate_speed"].Number();
            base.rotate_speed_max = (float)data["rotate_speed"].Number();
        }
        if (data["start_scale"].type == GonObject::FieldType::ARRAY)
        {
            base.start_scale_min = (float)data["start_scale"][0].Number();
            base.start_scale_max = (float)data["start_scale"][1].Number();
        }
        else
        {
            base.start_scale_min = (float)data["start_scale"].Number();
            base.start_scale_max = (float)data["start_scale"].Number();
        }
        if (data["end_scale"].type == GonObject::FieldType::ARRAY)
        {
            base.end_scale_min = (float)data["end_scale"][0].Number();
            base.end_scale_max = (float)data["end_scale"][1].Number();
        }
        else
        {
            base.end_scale_min = (float)data["end_scale"].Number();
            base.end_scale_max = (float)data["end_scale"].Number();
        }

        base.start_color.r = (float)data["start_color"][0].Number(255.0) / 255.0f;
        base.start_color.g = (float)data["start_color"][1].Number(255.0) / 255.0f;
        base.start_color.b = (float)data["start_color"][2].Number(255.0) / 255.0f;
        base.start_color.a = (float)data["start_color"][3].Number(255.0) / 255.0f;

        base.end_color.r = (float)data["end_color"][0].Number(255.0) / 255.0f;
        base.end_color.g = (float)data["end_color"][1].Number(255.0) / 255.0f;
        base.end_color.b = (float)data["end_color"][2].Number(255.0) / 255.0f;
        base.end_color.a = (float)data["end_color"][3].Number(255.0) / 255.0f;

        if (data["spawn_count"].type == GonObject::FieldType::ARRAY)
        {
            base.spawn_count_min = data["spawn_count"][0].Int();
            base.spawn_count_max = data["spawn_count"][1].Int();
        }
        else
        {
            base.spawn_count_min = data["spawn_count"].Int();
            base.spawn_count_max = data["spawn_count"].Int();
        }
        if (data["spawn_angle"].type == GonObject::FieldType::ARRAY)
        {
            base.spawn_angle_min = (float)data["spawn_angle"][0].Number();
            base.spawn_angle_max = (float)data["spawn_angle"][1].Number();
        }
        else
        {
            base.spawn_angle_min = (float)data["spawn_angle"].Number();
            base.spawn_angle_max = (float)data["spawn_angle"].Number();
        }

        gParticles.insert({ data.name, base });
    }
}

INTERNAL void UpdateParticles (std::vector<Particle>& particles)
{
    // Remove any particles that are no longer alive.
    // @Incomplete: ...

    for (auto& p: particles)
    {
        p.lifetime -= gApplication.delta_time;
        if (p.lifetime < 0.0f) continue;

        // Lerp some visual values based on the lifetime of the particle.
        p.draw.scale.current = Lerp(p.draw.scale.current, p.draw.scale.end, p.lifetime);
        p.draw.color.current = Lerp(p.draw.color.current, p.draw.color.end, p.lifetime);

        p.angle += p.rotate_speed * gApplication.delta_time;
        p.speed -= p.friction * gApplication.delta_time;

        if (p.speed < 0.0f) p.speed = 0.0f;

        Vec2 vel = RotateVec2({ p.speed, 0.0f, }, DegToRad(p.direction));

        p.pos.x += vel.x * gApplication.delta_time;
        p.pos.y += vel.y * gApplication.delta_time;
    }
}

INTERNAL void RenderParticles (std::vector<Particle>& particles)
{
    for (auto& p: particles)
    {
        if (p.lifetime <= 0.0f) continue;

        Vec2 scale = { p.draw.scale.current, p.draw.scale.current };
        Vec2 center = { TILE_W/2, TILE_H/2 };
        DrawImage("particle", p.pos.x,p.pos.y, scale, center, p.angle, FLIP_NONE, p.draw.color.current, &p.draw.clip);
    }
}
