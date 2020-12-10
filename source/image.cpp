INTERNAL void LoadImage (Image& image, std::string file_name)
{
    file_name = gAssetPath + file_name;

    constexpr int BPP = 4; // We force the image to be in 32-bit RGBA format!
    int w,h,bpp;
    unsigned char* data = stbi_load(file_name.c_str(), &w,&h,&bpp, BPP);
    if (!data) LOG_ERROR(ERR_MAX, "Failed to load image from file '%s'!", file_name.c_str());
    else
    {
        int pitch = w*BPP;
        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom((void*)data, w,h,BPP*8,pitch, SDL_PIXELFORMAT_RGBA32);
        if (!surface) LOG_ERROR(ERR_MAX, "Failed to create surface '%s'! (%s)", file_name.c_str(), SDL_GetError());
        else
        {
            image.texture = SDL_CreateTextureFromSurface(gWindow.renderer, surface);
            if (!image.texture) LOG_ERROR(ERR_MAX, "Failed to create texture '%s'! (%s)", file_name.c_str(), SDL_GetError());
            else
            {
                image.w = (float)w, image.h = (float)h;
                image.color = { 1,1,1,1 };
            }
            SDL_FreeSurface(surface);
        }
        stbi_image_free(data);
    }
}

INTERNAL void FreeImage (Image& image)
{
    SDL_DestroyTexture(image.texture);
    image.texture = NULL;
    image.w = 0.0f, image.h = 0.0f;
}
