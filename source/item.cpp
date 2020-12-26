GLOBAL std::map<std::string,ItemBase> gItems;

INTERNAL ItemBase& GetItem (std::string name)
{
    return gItems.at(name);
}

INTERNAL void InitItems ()
{
    gItems.clear();

    GonObject& gon = *GetAsset<AssetData>("items");
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Item data not formatted as object!");
        return;
    }

    // Go through each base item type and cache the data.
    for (auto& data: gon.children_array)
    {
        // Warn of items with duplicate names.
        if (gItems.count(data.name))
        {
            LOG_ERROR(ERR_MIN, "Warning duplicate item with the name %s!", data.name.c_str());
        }

        ItemBase base = {};

        base.pretty_name = data["pretty_name"].String();
        base.category = data["category"].String("none");

        if (data.Contains("image"))
        {
            int imagex = data["image"][0].Int(0);
            int imagey = data["image"][1].Int(0);

            base.clip.x = imagex*TILE_W;
            base.clip.y = imagey*TILE_H;
            base.clip.w = TILE_W;
            base.clip.h = TILE_H;
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

        base.place = data["place"].String("");
        base.hunger = (float)data["hunger"].Number(0.0);
        base.heal = (float)data["heal"].Number(0.0);

        if (data.Contains("recipe"))
        {
            base.recipe.equipment = data["recipe"]["equipment"].String("");
            if (data["recipe"].Contains("ingredients"))
            {
                for (int i=0; i<data["recipe"]["ingredients"].size(); ++i)
                {
                    ItemIngredient ingredient;
                    ingredient.type = data["recipe"]["ingredients"][i][0].String();
                    ingredient.amount = data["recipe"]["ingredients"][i][1].Int();
                    base.recipe.ingredients.push_back(ingredient);
                }
            }
        }

        base.stack = data["stack"].Int(99);
        base.damage = data["damage"].Int(1);

        gItems.insert({ data.name, base });
    }
}
