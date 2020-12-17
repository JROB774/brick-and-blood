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
    t->hits = base.hits;
    t->solid = base.solid;
    t->active = true;
    t->draw.clip.x = base.image.x*TILE_W;
    t->draw.clip.y = base.image.y*TILE_H;
    t->draw.clip.w = TILE_W;
    t->draw.clip.h = TILE_H;
    t->draw.angle.current = 0.0f;
    t->draw.angle.target = 0.0f;
    t->draw.color.current = base.color;
    t->draw.color.target = base.color;
}

INTERNAL void MapSpawnEntity (std::string type, int x, int y)
{
    // If the specified base type can't be found then we don't create an entity.
    if (!gEntities.count(type))
    {
        LOG_ERROR(ERR_MIN, "Could not create entity of unknown type: %s", type.c_str());
        return;
    }
    EntityBase& base = gEntities.at(type);

    // Check if the position contains a solid tile, if it does then do not spawn an entity.
    Tile* tile = MapGetTileAtPos(x,y);
    if (tile && tile->solid)
    {
        LOG_ERROR(ERR_MIN, "Could not place entity at (%d,%d), solid tile present!", x,y);
        return;
    }

    // Create the actual entity and add it to the manager.
    gMap.entities.push_back(Entity());
    Entity& e = gMap.entities.back();

    e.type = type;
    e.faction = base.faction;
    e.initiative = base.initiative;
    e.behavior = base.behavior;
    e.health = base.health;
    e.pos.x = x;
    e.pos.y = y;
    e.old_pos.x = x;
    e.old_pos.y = y;
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
            if (e.health > 0) return &e;
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

INTERNAL void InitMap ()
{
    // @Incomplete: In the future we want to load a map if there is one present. For now we just always generate.

    // Reset all of the map data.
    QuitMap();

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
}

INTERNAL void QuitMap ()
{
    gMap.entities.clear();
    gMap.particles.clear();

    memset(gMap.chunks, 0, sizeof(gMap.chunks));
}

INTERNAL void UpdateMap ()
{
    // Only update entites when the player performs an input (they are turn-based).
    if (gPlayer.update) UpdateEntities(gMap.entities);
    UpdateParticles(gMap.particles); // Always update particles, they are real-time.
}

INTERNAL void RenderMap ()
{
    // Render tiles.
    for (int iy=0; iy<WORLD_H; ++iy)
    {
        for (int ix=0; ix<WORLD_W; ++ix)
        {
            Chunk& chunk = gMap.chunks[iy][ix];
            for (int ty=0; ty<CHUNK_H; ++ty)
            {
                for (int tx=0; tx<CHUNK_W; ++tx)
                {
                    Tile& t = chunk.tiles[ty][tx];

                    // Smoothly lerp other values that have a current and a target for smoother visuals.
                    t.draw.angle.current = Lerp(t.draw.angle.current, t.draw.angle.target, gApplication.delta_time*TILE_TURN_SPEED);
                    t.draw.color.current = Lerp(t.draw.color.current, t.draw.color.target, gApplication.delta_time*TILE_COLOR_SPEED);

                    if (!t.active) continue; // Don't draw anything for empty tiles...

                    float x = (float)((ix*CHUNK_W)+tx) * TILE_W;
                    float y = (float)((iy*CHUNK_H)+ty) * TILE_H;

                    Vec2 center = { TILE_W/2, TILE_H/2 };
                    DrawImage("tile", x,y, {1,1}, center, t.draw.angle.current, FLIP_NONE, t.draw.color.current, &t.draw.clip);
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

        for (int iy=0; iy<WORLD_H; ++iy)
        {
            for (int ix=0; ix<WORLD_W; ++ix)
            {
                DrawRect(ix*w,iy*h,w,h, { 1,0,0,0.5f });
            }
        }
    }
}
