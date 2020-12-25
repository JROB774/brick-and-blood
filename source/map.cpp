INTERNAL void MapCheckForAndRemoveOutOfRangeEntities ()
{
    Entity* p = MapGetFirstEntityOfType("player");
    if (p) // We cannot despawn entities if there is no player to base it off of.
    {
        int minx = p->pos.x - MAX_ENTITY_ACTIVE_RADIUS;
        int miny = p->pos.y - MAX_ENTITY_ACTIVE_RADIUS;
        int maxx = p->pos.x + MAX_ENTITY_ACTIVE_RADIUS;
        int maxy = p->pos.y + MAX_ENTITY_ACTIVE_RADIUS;

        for (auto& e: gMap.entities)
        {
            if (e.pos.x <= minx || e.pos.x >= maxx || e.pos.y <= miny || e.pos.y >= maxy)
            {
                e.active = false;
            }
        }
    }
}

INTERNAL void MapPlaceTile (std::string type, int x, int y)
{
    // If the specified base type can't be found then we don't create a tile.
    if (!gTiles.count(type))
    {
        LOG_ERROR(ERR_MIN, "Could not create tile of unknown type: %s", type.c_str());
        return;
    }
    TileBase& base = gTiles.at(type);

    Tile* t = MapGetTileAtPos(x,y);
    if (!t) return;

    t->type = type;
    t->pos = { x,y };
    t->hits = base.hits;
    t->solid = base.solid;
    t->light = base.light;
    t->light_radius = base.light_radius;
    t->active = true;
    t->draw.clip.x = base.image.x*TILE_W;
    t->draw.clip.y = base.image.y*TILE_H;
    t->draw.clip.w = TILE_W;
    t->draw.clip.h = TILE_H;
    t->draw.scale.current = 1.0f;
    t->draw.scale.target = 1.0f;
    t->draw.angle.current = 0.0f;
    t->draw.angle.target = 0.0f;
    t->draw.color.current = base.color;
    t->draw.color.target = base.color;
}

INTERNAL void MapSpawnTile (std::string type, int x, int y)
{
    // If the specified base type can't be found then we don't create a tile.
    if (!gTiles.count(type))
    {
        LOG_ERROR(ERR_MIN, "Could not create tile of unknown type: %s", type.c_str());
        return;
    }
    TileBase& base = gTiles.at(type);

    Tile* t = MapGetTileAtPos(x,y);
    if (!t) return;

    t->type = type;
    t->pos = { x,y };
    t->hits = base.hits;
    t->solid = base.solid;
    t->light = base.light;
    t->light_radius = base.light_radius;
    t->active = true;
    t->draw.clip.x = base.image.x*TILE_W;
    t->draw.clip.y = base.image.y*TILE_H;
    t->draw.clip.w = TILE_W;
    t->draw.clip.h = TILE_H;
    t->draw.scale.current = TILE_SPAWN_SCALE;
    t->draw.scale.target = 1.0f;
    t->draw.angle.current = TILE_SPAWN_ANGLE;
    t->draw.angle.target = 0.0f;
    t->draw.color.current = base.color;
    t->draw.color.target = base.color;
}

INTERNAL void MapSpawnEntity (std::string type, int x, int y, std::string state_override)
{
    // Check if we have space for entities. If we don't see if any entities can be removed.
    if (gMap.entities.size() >= MAX_ENTITIES)
    {
        MapCheckForAndRemoveOutOfRangeEntities();
        if (gMap.entities.size() >= MAX_ENTITIES) // If still no space then don't spawn...
        {
            return;
        }
    }

    // Don't spawn an entity out of the map bounds.
    if (x < 0 || x >= WORLD_W_IN_TILES || y < 0 || y >= WORLD_H_IN_TILES)
    {
        return;
    }

    // If the specified base type can't be found then we don't create an entity.
    if (!gEntities.count(type))
    {
        LOG_ERROR(ERR_MIN, "Could not create entity of unknown type: %s", type.c_str());
        return;
    }
    EntityBase& base = gEntities.at(type);

    // Check if the position contains a solid tile, if it does then do not spawn an entity.
    Tile* tile = MapGetTileAtPos(x,y);
    if (tile && tile->solid) return;
    // Check if the position contains an entity, if it does then do not spawn an entity.
    Entity* entity = MapGetEntityAtPos(x,y);
    if (entity) return;

    // Create the actual entity and add it to the manager.
    gMap.entities.push_back(Entity());
    Entity& e = gMap.entities.back();

    e.type = type;
    e.faction = base.faction;
    e.state = (state_override.empty()) ? base.state : state_override;
    e.initiative = base.initiative;
    e.behavior_type = base.behavior_type;
    e.behavior = base.behavior;
    e.health = base.health;
    e.damage = base.damage;
    e.light = base.light;
    e.light_radius = base.light_radius;
    e.pos.x = x;
    e.pos.y = y;
    e.old_pos.x = x;
    e.old_pos.y = y;
    e.active = true;
    e.draw.pos.x = (float)(x*TILE_W);
    e.draw.pos.y = (float)(y*TILE_H);
    e.draw.clip.x = base.image.x*TILE_W;
    e.draw.clip.y = base.image.y*TILE_H;
    e.draw.clip.w = TILE_W;
    e.draw.clip.h = TILE_H;
    e.draw.angle.current = 0.0f;
    e.draw.angle.target = 0.0f;
    e.draw.color.current = base.color;
    e.draw.color.target = base.color;
}

INTERNAL void MapSpawnParticles (std::string type, Rect region)
{
    // If the specified base type can't be found then we don't create particles.
    if (!gParticles.count(type))
    {
        LOG_ERROR(ERR_MIN, "Could not create particles of unknown type: %s", type.c_str());
        return;
    }
    ParticleBase& base = gParticles.at(type);

    int count = RandomRange(base.spawn_count_min, base.spawn_count_max);
    for (int i=0; i<count; ++i)
    {
        // Create the actual particle and add it to the manager.
        gMap.particles.push_back(Particle());
        Particle& p = gMap.particles.back();

        p.type = type;
        p.pos.x = RandomFloatRange(region.x, region.x+region.w) - (TILE_W/2);
        p.pos.y = RandomFloatRange(region.y, region.y+region.h) - (TILE_H/2);
        p.lifetime = RandomFloatRange(base.lifetime_min, base.lifetime_max);
        p.direction = RandomFloatRange(base.spawn_angle_min, base.spawn_angle_max);
        p.speed = RandomFloatRange(base.speed_min, base.speed_max);
        p.friction = RandomFloatRange(base.friction_min, base.friction_max);
        p.angle = RandomFloatRange(base.angle_min, base.angle_max);
        p.rotate_speed = RandomFloatRange(base.rotate_speed_min, base.rotate_speed_max);
        p.draw.clip = base.clips.at(RandomRange(0, (int)base.clips.size()-1));
        p.draw.scale.current = RandomFloatRange(base.start_scale_min, base.start_scale_max);
        p.draw.scale.end = RandomFloatRange(base.end_scale_min, base.end_scale_max);
        p.draw.color.current = base.start_color;
        p.draw.color.end = base.end_color;
    }
}

INTERNAL Tile* MapGetTileAtPos (int x, int y)
{
    if (x < 0 || y < 0 || x >= WORLD_W_IN_TILES || y >= WORLD_H_IN_TILES) return NULL;

    int cx = x / CHUNK_W;
    int cy = y / CHUNK_H;
    Chunk& chunk = gMap.chunks[cy][cx];
    int tx = x % CHUNK_W;
    int ty = y % CHUNK_H;
    return &chunk.tiles[ty][tx];
}

INTERNAL Entity* MapGetEntityAtPos (int x, int y)
{
    for (auto& e: gMap.entities)
    {
        if (e.pos.x == x && e.pos.y == y)
        {
            if (e.active) return &e;
            break;
        }
    }
    return NULL;
}

INTERNAL Entity* MapGetFirstEntityOfType (std::string type)
{
    Entity* entity = NULL;
    for (auto& e: gMap.entities)
    {
        if (e.type == type)
        {
            entity = &e;
            break;
        }
    }
    return entity;
}

INTERNAL std::vector<Tile*> MapGetAllLoadedTilesOfType (std::string type)
{
    std::vector<Tile*> tiles;

    // We use the culled/loaded area for retrieving the tiles.
    int minx = 0;
    int miny = 0;
    int maxx = 0;
    int maxy = 0;

    Entity* p = MapGetFirstEntityOfType("player");
    if (!p) return tiles;

    int cx = p->pos.x / CHUNK_W;
    int cy = p->pos.y / CHUNK_H;

    minx = std::clamp(cx-3, 0, WORLD_W-1);
    miny = std::clamp(cy-3, 0, WORLD_H-1);
    maxx = std::clamp(cx+3, 0, WORLD_W-1);
    maxy = std::clamp(cy+3, 0, WORLD_H-1);

    for (int iy=miny; iy<=maxy; ++iy)
    {
        for (int ix=minx; ix<=maxx; ++ix)
        {
            Chunk& chunk = gMap.chunks[iy][ix];
            for (int ty=0; ty<CHUNK_H; ++ty)
            {
                for (int tx=0; tx<CHUNK_W; ++tx)
                {
                    Tile& t = chunk.tiles[ty][tx];
                    if (t.type == type) tiles.push_back(&t);
                }
            }
        }
    }

    return tiles;
}

INTERNAL int MapGetNumberOfEntitiesOfFaction (std::string faction)
{

}

INTERNAL void MapRandomlySpawnEntities ()
{
    Entity* p = MapGetFirstEntityOfType("player");
    if (p) // We cannot spawn entities if there is no player to base it off of.
    {
        std::vector<std::string> to_spawn;

        if (IsDay()) // Spawn animals during the day.
        {
            to_spawn = GetAllEntityTypesOfFaction("animal");

            constexpr int CHANCES_TO_SPAWN = 3;
            for (int i=0; i<CHANCES_TO_SPAWN; ++i)
            {
                if (RandomRange(1,100) <= 5)
                {
                    // Determine type to spawn.
                    std::string type = to_spawn.at(RandomRange(0,(int)to_spawn.size()-1));

                    // Determine where to spawn the entities.
                    Vec2 spawn_off = { (float)RandomRange(MIN_ENTITY_SPAWN_RADIUS,MAX_ENTITY_SPAWN_RADIUS),0 };
                    spawn_off = RotateVec2(spawn_off, DegToRad(RandomFloatRange(0.0f,360.0f)));

                    int sx = p->pos.x + (int)spawn_off.x;
                    int sy = p->pos.y + (int)spawn_off.y;

                    // Determine how many to spawn.
                    int amount = RandomRange(3,7);
                    for (int j=0; j<amount; ++j)
                    {
                        int x = RandomRange(sx-5,sx+5);
                        int y = RandomRange(sy-5,sy+5);
                        MapSpawnEntity(type,x,y);
                    }
                }
            }
        }
        else if (IsNight()) // Spawn monsters at night.
        {
            to_spawn = GetAllEntityTypesOfFaction("monster");

            constexpr int CHANCES_TO_SPAWN = 3;
            for (int i=0; i<CHANCES_TO_SPAWN; ++i)
            {
                if (RandomRange(1,100) <= 1)
                {
                    // Determine type to spawn.
                    std::string type = to_spawn.at(RandomRange(0,(int)to_spawn.size()-1));

                    // Spawn monsters slightly off of the screen.
                    int xoffset = 0;
                    int yoffset = 0;

                    int width = (int)ceilf((float)WINDOW_SCREEN_W / (float)TILE_W);
                    int height = (int)ceilf((float)WINDOW_SCREEN_H / (float)TILE_H);

                    xoffset = RandomRange((width/2)+2, width);
                    yoffset = RandomRange((height/2)+2, height);

                    if (Random() % 2 == 0) xoffset = -xoffset;
                    if (Random() % 2 == 0) yoffset = -yoffset;

                    // Spawn the entity.
                    int sx = p->pos.x + xoffset;
                    int sy = p->pos.y + yoffset;
                    MapSpawnEntity(type,sx,sy);
                }
            }
        }
    }
}

INTERNAL void InitMap ()
{
    // @Incomplete: In the future we want to load a map if there is one present. For now we just always generate.

    // Reset all of the map data.
    QuitMap();

    gMap.chunks.resize(WORLD_H);
    for (auto& row: gMap.chunks) row.resize(WORLD_W);

    // Load in all of the available chunks that can be used to generate a map.
    GonObject& gon = (*GetAsset<AssetData>("chunks/overworld"))["chunks"];

    // Generate a world map using the loaded in chunks.
    for (int iy=0; iy<WORLD_H; ++iy)
    {
        for (int ix=0; ix<WORLD_W; ++ix)
        {
            int chunk_index = RandomRange(0,gon.size()-1);
            GonObject& chunk_data = gon[chunk_index];

            // Load tiles.
            if (chunk_data.Contains("tiles"))
            {
                for (auto& tile: chunk_data["tiles"].children_array)
                {
                    int x = tile[0].Int(), y = tile[1].Int();
                    std::string name = tile[2].String();
                    MapPlaceTile(name, (ix*CHUNK_W)+x,(iy*CHUNK_H)+y);
                }
            }
            // Load entites.
            if (chunk_data.Contains("entities"))
            {
                for (auto& tile: chunk_data["entities"].children_array)
                {
                    int x = tile[0].Int(), y = tile[1].Int();
                    std::string name = tile[2].String();
                    MapSpawnEntity(name, (ix*CHUNK_W)+x, (iy*CHUNK_H)+y);
                }
            }
        }
    }

    // Spawn the player in the center of the map.
    int px = (WORLD_W*CHUNK_W) / 2;
    int py = (WORLD_H*CHUNK_H) / 2;
    MapSpawnEntity("player",px,py);

    // Spawn in some initial entities.
    MapRandomlySpawnEntities();
}

INTERNAL void QuitMap ()
{
    gMap.chunks.clear();
    gMap.entities.clear();
    gMap.particles.clear();
}

INTERNAL void UpdateMap ()
{
    // Only update certain bits of logic when the player interacts with the world.
    if (gPlayer.update)
    {
        MapRandomlySpawnEntities();
        UpdateEntities(gMap.entities);

        // If it's day time kill all of the monsters from the night.
        if (IsDay())
        {
            for (auto& e: gMap.entities)
            {
                if (e.faction == "monster")
                {
                    KillEntity(e);
                }
            }
        }
    }

    // Always update particles, they should run in  real-time.
    UpdateParticles(gMap.particles);
}

INTERNAL void RenderMap ()
{
    // Render Tiles

    // We determine a range of chunks around the player so we can cull the rest.
    // Avoiding rendering the whole map when only a small fraction can be seen.
    int minx = 0;
    int miny = 0;
    int maxx = WORLD_W-1;
    int maxy = WORLD_H-1;

    Entity* p = MapGetFirstEntityOfType("player");
    if (p) // We need the player to do this. If we can't find one we just fallback to the whole map.
    {
        int cx = p->pos.x / CHUNK_W;
        int cy = p->pos.y / CHUNK_H;

        minx = std::clamp(cx-3, 0, WORLD_W-1);
        miny = std::clamp(cy-3, 0, WORLD_H-1);
        maxx = std::clamp(cx+3, 0, WORLD_W-1);
        maxy = std::clamp(cy+3, 0, WORLD_H-1);
    }

    for (int iy=miny; iy<=maxy; ++iy)
    {
        for (int ix=minx; ix<=maxx; ++ix)
        {
            Chunk& chunk = gMap.chunks[iy][ix];
            for (int ty=0; ty<CHUNK_H; ++ty)
            {
                for (int tx=0; tx<CHUNK_W; ++tx)
                {
                    Tile& t = chunk.tiles[ty][tx];

                    // Smoothly lerp other values that have a current and a target for smoother visuals.
                    t.draw.scale.current = Lerp(t.draw.scale.current, t.draw.scale.target, gApplication.delta_time*TILE_SCALE_SPEED);
                    t.draw.angle.current = Lerp(t.draw.angle.current, t.draw.angle.target, gApplication.delta_time*TILE_TURN_SPEED);
                    t.draw.color.current = Lerp(t.draw.color.current, t.draw.color.target, gApplication.delta_time*TILE_COLOR_SPEED);

                    if (!t.active) continue; // Don't draw anything for empty tiles...

                    float x = (float)((ix*CHUNK_W)+tx) * TILE_W;
                    float y = (float)((iy*CHUNK_H)+ty) * TILE_H;

                    // Adjust the X and Y for the current scale of the tile so it remains centered.
                    x -= (TILE_W * (t.draw.scale.current - 1.0f)) / 2;
                    y -= (TILE_H * (t.draw.scale.current - 1.0f)) / 2;

                    Vec2 scale = { t.draw.scale.current, t.draw.scale.current };
                    Vec2 center = { TILE_W/2, TILE_H/2 };
                    DrawImage("tile", x,y, scale, center, t.draw.angle.current, FLIP_NONE, t.draw.color.current, &t.draw.clip);

                    // If it's a light source, draw a light.
                    if (t.light)
                    {
                        float lx = (float)(((ix*CHUNK_W)+tx) * TILE_W) + (TILE_W/2);
                        float ly = (float)(((iy*CHUNK_H)+ty) * TILE_H) + (TILE_H/2);
                        DrawLight(lx,ly,t.light_radius);
                    }
                }
            }
        }
    }

    RenderEntities(gMap.entities);
    RenderParticles(gMap.particles);

    // If in debug mode show chunk boudaries.
    if (gApplication.debug)
    {
        float w = (CHUNK_W*TILE_W);
        float h = (CHUNK_H*TILE_H);

        for (int iy=miny; iy<=maxy; ++iy)
        {
            for (int ix=minx; ix<=maxx; ++ix)
            {
                DrawRect(ix*w,iy*h,w,h, { 1,0,0,0.5f });
            }
        }
    }
}
