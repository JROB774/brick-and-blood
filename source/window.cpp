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

    // OpenGL Attributes

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // We aren't using either of these buffers (yet) so no need for them to be set.
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

    // The window starts off hidden so we don't have a white window displaying whilst all the resources load and systems initialize.
    gWindow.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, gWindow.width,gWindow.height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);
    if (!gWindow.window)
    {
        LOG_ERROR(ERR_MAX, "Failed to create window! (%s)", SDL_GetError());
        return false;
    }

    gWindow.context = SDL_GL_CreateContext(gWindow.window);
    if (!gWindow.context)
    {
        LOG_ERROR(ERR_MAX, "Failed to create OpenGL context! (%s)", SDL_GetError());
        return false;
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        LOG_ERROR(ERR_MAX, "Failed to load OpenGL procedures!");
        return false;
    }

    SDL_SetWindowMinimumSize(gWindow.window, WINDOW_SCREEN_W,WINDOW_SCREEN_H);

    SetFullscreen(gSettings.fullscreen);

    return true;
}

INTERNAL void QuitWindow ()
{
    SDL_GL_DeleteContext(gWindow.context);
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
    if (gWindow.fullscreen) return gWindow.width;
    int windoww,windowh;
    SDL_GetWindowSize(gWindow.window, &windoww,&windowh);
    return windoww;
}

INTERNAL int GetWindowHeight ()
{
    if (gWindow.fullscreen) return gWindow.height;
    int windoww,windowh;
    SDL_GetWindowSize(gWindow.window, &windoww,&windowh);
    return windowh;
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

    // OpenGL Attributes

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // We aren't using either of these buffers (yet) so no need for them to be set.
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

    // The window starts off hidden so we don't have a white window displaying whilst all the resources load and systems initialize.
    gWindow.window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, gWindow.width,gWindow.height, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL|SDL_WINDOW_HIDDEN);
    if (!gWindow.window)
    {
        LOG_ERROR(ERR_MAX, "Failed to create virtual window! (%s)", SDL_GetError());
        return false;
    }

    gWindow.context = SDL_GL_CreateContext(gWindow.window);
    if (!gWindow.context)
    {
        LOG_ERROR(ERR_MAX, "Failed to create OpenGL context! (%s)", SDL_GetError());
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

INTERNAL void SetWindowSize (int width, int height)
{
    // Nothing...
}

#endif // PLATFORM_WEB =========================================================

INTERNAL void ClearWindow (Vec4 color)
{
    glClearColor(color.r,color.g,color.b,color.a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}

INTERNAL void RefreshWindow ()
{
    SDL_GL_SwapWindow(gWindow.window);
}

INTERNAL void SetViewport ()
{
    // @Incomplete: ...
}

INTERNAL void UnsetViewport ()
{
    // @Incomplete: ...
}
