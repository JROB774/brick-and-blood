INTERNAL void LoadFont (Font& font, float cw, float ch, std::string file_name)
{
    LoadImage(font.image, file_name);

    int x = 0;
    int y = 0;
    int w = (int)cw;
    int h = (int)ch;

    for (int i=0; i<FONT_CHAR_COUNT; ++i)
    {
        font.bounds[i] = { x,y,w,h };
        x += w;
        if (x >= font.image.w)
        {
            x = 0;
            y += h;
        }
    }

    font.charw = cw;
    font.charh = ch;
}

INTERNAL void FreeFont (Font& font)
{
    FreeImage(font.image);
}

INTERNAL float GetTextLineWidth (Font& font, std::string text, int line)
{
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
            linewidth += font.charw;
        }
    }
    return linewidth;
}

INTERNAL float GetTextWidth (Font& font, std::string text)
{
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
            linewidth += font.charw;
        }
    }
    return std::max(width, linewidth);
}

INTERNAL float GetTextHeight (Font& font, std::string text)
{
    float height = font.charh;
    for (int i=0; i<text.length(); ++i)
    {
        if (text[i] == '\n')
        {
            height += font.charh;
        }
    }
    return height;
}
