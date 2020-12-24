GLOBAL struct Lighting
{
    float target_intensity;
    float intensity;
    float offset;
    SDL_Texture* mask;
    bool enabled;

} gLighting;

INTERNAL void InitLighting ()
{
    gLighting.target_intensity = 0.0f;
    gLighting.intensity = 0.0f;

    gLighting.offset = 0.0f;

    gLighting.enabled = false;

    gLighting.mask = SDL_CreateTexture(gWindow.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_SCREEN_W, WINDOW_SCREEN_H);
    if (!gLighting.mask)
    {
        LOG_ERROR(ERR_MAX, "Failed to create lighting mask! (%s)", SDL_GetError());
        return;
    }
    else
    {
        if (SDL_SetTextureBlendMode(gLighting.mask, SDL_BLENDMODE_MOD) < 0)
        {
            LOG_ERROR(ERR_MAX, "Failed to set lighting blend mode! (%s)", SDL_GetError());
            return;
        }
    }
}

INTERNAL void QuitLighting ()
{
    gLighting.enabled = false;
    SDL_DestroyTexture(gLighting.mask);
}

INTERNAL void BeginLighting ()
{
    // Smoothly lerp the lighting intensity.
    gLighting.intensity = Lerp(gLighting.intensity, gLighting.target_intensity, gApplication.delta_time*2.0f);

    // Fill the lighting mask with ambient shadow.
    U8 intensity = (U8)((1.0f-gLighting.intensity)*255.0f);
    SDL_SetRenderTarget(gWindow.renderer, gLighting.mask);
    SDL_SetRenderDrawColor(gWindow.renderer, intensity,intensity,intensity,255);
    SDL_RenderClear(gWindow.renderer);
    SDL_SetRenderTarget(gWindow.renderer, NULL);

    // This is done so the lights don't flicker too fast.
    if (gApplication.frame % 3 == 0) gLighting.offset = RandomFloatRange(8.0f,16.0f);
}

INTERNAL void EndLighting ()
{
    // If lighting is enabled then draw the lighting mask to the screen.
    if (gLighting.enabled) SDL_RenderCopyF(gWindow.renderer, gLighting.mask, NULL, NULL);
}

INTERNAL void EnableLighting (bool enable)
{
    gLighting.enabled = enable;
}

INTERNAL void SetLightingIntensity (float intensity)
{
    gLighting.target_intensity = std::clamp(intensity, 0.0f,1.0f);
}

INTERNAL void DrawLight (float x, float y, float r)
{
    r += gLighting.offset;
    SDL_SetRenderTarget(gWindow.renderer, gLighting.mask);
    Vec2 scale = { (r*4)/512.0f, (r*4)/512.0f };
    DrawImage("light", x-r,y-r, scale, {0,0}, 0.0f, FLIP_NONE);
    SDL_SetRenderTarget(gWindow.renderer, NULL);
}
