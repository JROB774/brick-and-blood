std::map<std::string,TileBase> gTiles;

INTERNAL void InitTiles ()
{
    gTiles.clear();

    GonObject& gon = *GetAsset<AssetData>("tiles");
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Tile data not formatted as object!");
        return;
    }

    // Go through each base tile type and cache the data.
    for (auto& data: gon.children_array)
    {
        // Warn of tiles with duplicate names.
        if (gTiles.count(data.name))
        {
            LOG_ERROR(ERR_MIN, "Warning duplicate tile with the name %s!", data.name.c_str());
        }

        TileBase base = {};

        base.hits = data["hits"].Int(TILE_INDESTRUCTIBLE);

        if (data.Contains("image"))
        {
            base.image.x = data["image"][0].Int(0);
            base.image.y = data["image"][1].Int(0);
        }

        if (data.Contains("color"))
        {
            base.color.r = (float)data["color"][0].Number(255.0) / 255.0f;
            base.color.g = (float)data["color"][1].Number(255.0) / 255.0f;
            base.color.b = (float)data["color"][2].Number(255.0) / 255.0f;
            base.color.a = (float)data["color"][3].Number(255.0) / 255.0f;
        }
        else
        {
            base.color = { 1,1,1,1 };
        }

        base.solid = data["solid"].Bool(true);

        if (data.Contains("particle_hit"))
        {
            if (data["particle_hit"].type == GonObject::FieldType::ARRAY)
            {
                for (int i=0; i<data["particle_hit"].size(); ++i)
                {
                    base.particle_hit.push_back(data["particle_hit"][i].String());
                }
            }
            else
            {
                base.particle_hit.push_back(data["particle_hit"].String());
            }
        }
        if (data.Contains("particle_break"))
        {
            if (data["particle_break"].type == GonObject::FieldType::ARRAY)
            {
                for (int i=0; i<data["particle_break"].size(); ++i)
                {
                    base.particle_break.push_back(data["particle_break"][i].String());
                }
            }
            else
            {
                base.particle_break.push_back(data["particle_break"].String());
            }
        }

        if (data.Contains("sound_hit"))
        {
            if (data["sound_hit"].type == GonObject::FieldType::ARRAY)
            {
                for (int i=0; i<data["sound_hit"].size(); ++i)
                {
                    base.sound_hit.push_back(data["sound_hit"][i].String());
                }
            }
            else
            {
                base.sound_hit.push_back(data["sound_hit"].String());
            }
        }

        if (data.Contains("drops"))
        {
            for (int i=0; i<data["drops"].size(); ++i)
            {
                TileDrops drops;
                drops.type = data["drops"][i][0].String();
                drops.min  = data["drops"][i][1].Int();
                drops.max  = data["drops"][i][2].Int();
                base.drops.push_back(drops);
            }
        }

        gTiles.insert({ data.name, base });
    }
}

//
// HELPERS
//

INTERNAL void DamageTile (int x, int y)
{
    Tile* tile = MapGetTileAtPos(x,y);
    if (!tile) return;

    Tile& t = *tile;

    if (t.hits == TILE_INDESTRUCTIBLE) return;

    t.draw.angle.current = TILE_HIT_ANGLE;
    t.hits--;
    if (t.hits <= 0) t.active = false;

    // Handle either hit or death visual and sound effects.
    TileBase& base = gTiles.at(t.type);

    Rect particle_region = { (float)(x*TILE_W)+(TILE_W/2),(float)(y*TILE_H)+(TILE_H/2),0,0 };
    if (t.hits > 0) for (auto& p: base.particle_hit) MapSpawnParticles(p, particle_region);
    else for (auto& p: base.particle_break) MapSpawnParticles(p, particle_region);

    if (!base.sound_hit.empty())
    {
        PlaySound(base.sound_hit.at(RandomRange(0,(int)base.sound_hit.size()-1)));
    }

    // Handle picking up the drops on break.
    if (t.hits <= 0)
    {
        if (!base.drops.empty())
        {
            for (auto& drop: base.drops)
            {
                int quantity = RandomRange(drop.min,drop.max);
                PlayerPickUp(drop.type, quantity);
            }
        }
    }
}
