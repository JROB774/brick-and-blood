INTERNAL void LoadFont (Font& font, std::string file_name)
{
    GonObject gon = GonObject::Load(file_name);
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Font not a GON object! (%s)", file_name.c_str());
        return;
    }

    font.image = gon["image"].String();
    font.charw = static_cast<float>(gon["char_width"].Number());
    font.charh = static_cast<float>(gon["char_height"].Number());

    Image* image = GetAsset<AssetImage>(font.image);
    if (!image)
    {
        LOG_ERROR(ERR_MAX, "Failed to load font image! (%s)", file_name.c_str());
        return;
    }

    int x = 0;
    int y = 0;
    int w = (int)font.charw;
    int h = (int)font.charh;

    for (int i=0; i<FONT_CHAR_COUNT; ++i)
    {
        font.bounds[i] = { x,y,w,h };
        x += w;
        if (x >= image->w)
        {
            x = 0;
            y += h;
        }
    }
}

INTERNAL void FreeFont (Font& font)
{
    // Nothing...
}

INTERNAL float GetTextLineWidth (std::string font_name, std::string text, int line)
{
    Font* font = GetAsset<AssetFont>(font_name);
    if (!font) return 0.0f;

    float linewidth = 0;
    int lineindex = 0;
    for (int i=0; i<text.length(); ++i)
    {
        if (text[i] == '\n')
        {
            if (lineindex++ == line) return linewidth;
            else linewidth = 0;
        }
        else
        {
            linewidth += font->charw;
        }
    }
    return linewidth;
}

INTERNAL float GetTextWidth (std::string font_name, std::string text)
{
    Font* font = GetAsset<AssetFont>(font_name);
    if (!font) return 0.0f;

    float linewidth = 0;
    float width = 0;
    for (int i=0; i<text.length(); ++i)
    {
        if (text[i] == '\n')
        {
            width = std::max(width, linewidth);
            linewidth = 0;
        }
        else
        {
            linewidth += font->charw;
        }
    }
    return std::max(width, linewidth);
}

INTERNAL float GetTextHeight (std::string font_name, std::string text)
{
    Font* font = GetAsset<AssetFont>(font_name);
    if (!font) return 0.0f;

    float height = font->charh;
    for (int i=0; i<text.length(); ++i)
    {
        if (text[i] == '\n')
        {
            height += font->charh;
        }
    }
    return height;
}
