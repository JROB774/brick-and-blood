struct TileBase
{
    int hits;
    struct { int x,y; } image;
    Vec4 color;
    bool solid;
};

std::map<std::string,TileBase> gTiles;

INTERNAL void InitTiles ()
{
    GonObject& gon = *GetAsset<AssetData>("tiles");
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Tile data not formatted as object!");
        return;
    }

    // Go through each base tile type and cache the data.
    for (auto& data: gon.children_array)
    {
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

        gTiles.insert({ data.name, base });
    }
}
