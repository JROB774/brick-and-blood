INTERNAL void LoadImage (Image& image, std::string file_name)
{
    constexpr int BPP = 4; // We force our images to be 32-bit RGBA (4 components).
    int w,h,bpp;
    unsigned char* data = stbi_load(file_name.c_str(), &w,&h,&bpp,BPP);
    if (!data)
    {
        LOG_ERROR(ERR_MAX, "Failed to load image file! (%s)", file_name.c_str());
        return;
    }

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &image.texture);
    glBindTexture(GL_TEXTURE_2D, image.texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,   GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,   GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w,h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    image.w = static_cast<float>(w);
    image.h = static_cast<float>(h);

    stbi_image_free(data);
}

INTERNAL void FreeImage (Image& image)
{
    glDeleteTextures(1, &image.texture);
    image.w = 0.0f, image.h = 0.0f;
}
