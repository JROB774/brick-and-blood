#if defined(PLATFORM_WIN32) // =================================================

INTERNAL bool InitWindow ()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LOG_ERROR(ERR_MAX, "Failed to initialize SDL! (%s)", SDL_GetError());
        return false;
    }

    gWindow.width = gSettings.window_width;
    gWindow.height = gSettings.window_height;

    // The window starts off hidden so we don't have a white window displaying whilst all the resources load and systems initialize.
    gWindow.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, gWindow.width,gWindow.height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_HIDDEN);
    if (!gWindow.window)
    {
        LOG_ERROR(ERR_MAX, "Failed to create window! (%s)", SDL_GetError());
        return false;
    }

    gWindow.renderer = SDL_CreateRenderer(gWindow.window, -1, SDL_RENDERER_ACCELERATED);
    if (!gWindow.renderer)
    {
        LOG_ERROR(ERR_MAX, "Failed to create renderer! (%s)", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(gWindow.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetWindowMinimumSize(gWindow.window, WINDOW_SCREEN_W,WINDOW_SCREEN_H);

    SetFullscreen(gSettings.fullscreen);

    return true;
}

INTERNAL void QuitWindow ()
{
    SDL_DestroyRenderer(gWindow.renderer);
    SDL_DestroyWindow(gWindow.window);
    SDL_Quit();
}

INTERNAL void ShowWindow ()
{
    SDL_ShowWindow(gWindow.window);
}

INTERNAL void HideWindow ()
{
    SDL_HideWindow(gWindow.window);
}

INTERNAL void SetFullscreen (bool enable)
{
    if (enable) gWindow.width = GetWindowWidth();
    if (enable) gWindow.height = GetWindowHeight();

    gWindow.fullscreen = enable;
    SDL_SetWindowFullscreen(gWindow.window, (gWindow.fullscreen) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    SDL_ShowCursor((gWindow.fullscreen) ? SDL_DISABLE : SDL_ENABLE);
}

INTERNAL bool IsFullscreen ()
{
    return gWindow.fullscreen;
}

INTERNAL int GetWindowWidth ()
{
    // if (gWindow.fullscreen) return gWindow.width;
    int windoww,windowh;
    SDL_GetWindowSize(gWindow.window, &windoww,&windowh);
    return windoww;
}
INTERNAL int GetWindowHeight ()
{
    // if (gWindow.fullscreen) return gWindow.height;
    int windoww,windowh;
    SDL_GetWindowSize(gWindow.window, &windoww,&windowh);
    return windowh;
}

// Used for saving settings data!
INTERNAL int GetCachedWindowWidth ()
{
    if (gWindow.fullscreen) return gWindow.width;
    return GetWindowWidth();
}
INTERNAL int GetCachedWindowHeight ()
{
    if (gWindow.fullscreen) return gWindow.height;
    return GetWindowHeight();
}

INTERNAL void SetWindowSize (int width, int height)
{
    SDL_SetWindowSize(gWindow.window, width, height);
}

#endif // PLATFORM_WIN32 =======================================================

#if defined(PLATFORM_WEB) // ===================================================

INTERNAL bool InitWindow ()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        LOG_ERROR(ERR_MAX, "Failed to initialize SDL! (%s)", SDL_GetError());
        return false;
    }

    // These will never change or be used when in the web build of the game!
    gWindow.width = WINDOW_START_W;
    gWindow.height = WINDOW_START_H;
    gWindow.running = true;
    gWindow.fullscreen = false;

    if (SDL_CreateWindowAndRenderer(WINDOW_START_W,WINDOW_START_H, 0, &gWindow.window, &gWindow.renderer) < 0)
    {
        LOG_ERROR(ERR_MAX, "Failed to create window and/or renderer! (%s)", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawBlendMode(gWindow.renderer, SDL_BLENDMODE_BLEND);

    return true;
}

INTERNAL void QuitWindow ()
{
    // Nothing...
}

INTERNAL void ShowWindow ()
{
    // Nothing...
}

INTERNAL void HideWindow ()
{
    // Nothing...
}

INTERNAL void SetFullscreen (bool enable)
{
    // Nothing...
}

INTERNAL bool IsFullscreen ()
{
    return false;
}

INTERNAL int GetWindowWidth ()
{
    return WINDOW_START_W;
}
INTERNAL int GetWindowHeight ()
{
    return WINDOW_START_H;
}

// Used for saving settings data!
INTERNAL int GetCachedWindowWidth ()
{
    return WINDOW_START_W;
}
INTERNAL int GetCachedWindowHeight ()
{
    return WINDOW_START_H;
}

INTERNAL void SetWindowSize (int width, int height)
{
    // Nothing...
}

#endif // PLATFORM_WEB =========================================================

INTERNAL void ClearWindow (Vec4 color)
{
    SDL_Color c = ColorToSDLColor(color);
    SDL_SetRenderDrawColor(gWindow.renderer, c.r,c.g,c.b,c.a);
    SDL_RenderClear(gWindow.renderer);
}

INTERNAL void RefreshWindow ()
{
    SDL_RenderPresent(gWindow.renderer);
}

INTERNAL void SetViewport ()
{
    int windoww,windowh;
    SDL_GetWindowSize(gWindow.window, &windoww,&windowh);

    int sx = (int)floorf(((float)windoww / (float)WINDOW_SCREEN_W));
    int sy = (int)floorf(((float)windowh / (float)WINDOW_SCREEN_H));

    // Determine the smallest scale and use that.
    float scale = (float)((sx < sy) ? sx : sy);

    SDL_RenderSetScale(gWindow.renderer, scale,scale);

    SDL_Rect viewport;

    viewport.x = ((windoww - (WINDOW_SCREEN_W * (int)scale)) / 2) / (int)scale;
    viewport.y = ((windowh - (WINDOW_SCREEN_H * (int)scale)) / 2) / (int)scale;
    viewport.w = WINDOW_SCREEN_W;
    viewport.h = WINDOW_SCREEN_H;

    SDL_RenderSetViewport(gWindow.renderer, &viewport);
}

INTERNAL void UnsetViewport ()
{
    SDL_RenderSetViewport(gWindow.renderer, NULL);
    SDL_RenderSetScale(gWindow.renderer, 1,1);
}
