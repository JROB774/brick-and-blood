#ifndef IMAGE_HPP
#define IMAGE_HPP

struct Image
{
    SDL_Texture* texture;
    float w, h;
};

// SHOULD NOT BE USED DIRECTLY -- USE THE ASSET MANAGER INSTEAD!!!
INTERNAL void LoadImage (Image& image, std::string file_name);
INTERNAL void FreeImage (Image& image);

#endif /* IMAGE_HPP */
