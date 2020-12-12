#pragma once

GLOBAL constexpr int FONT_CHAR_COUNT = 256;

struct Font
{
    SDL_Rect bounds[FONT_CHAR_COUNT];
    std::string image;
    float charw;
    float charh;
};

// SHOULD NOT BE USED DIRECTLY -- USE THE ASSET MANAGER INSTEAD!!!
INTERNAL void LoadFont (Font& font, std::string file_name);
INTERNAL void FreeFont (Font& font);

//
// INTERFACE
//

INTERNAL float GetTextLineWidth (std::string font_name, std::string text, int line);
INTERNAL float GetTextWidth     (std::string font_name, std::string text);
INTERNAL float GetTextHeight    (std::string font_name, std::string text);
