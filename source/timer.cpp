INTERNAL float CounterToSeconds (U64 counter)
{
    return ((float)counter / (float)gTimer.performance_frequency);
}

#if defined(PLATFORM_WIN32) // =================================================

GLOBAL constexpr int FRAMERATE = 60;

INTERNAL void InitFrameTimer ()
{
    gTimer.performance_frequency = SDL_GetPerformanceFrequency();
    gTimer.last_counter          = SDL_GetPerformanceCounter();
    gTimer.end_counter           = 0;
    gTimer.current_fps           = 0.0f;
    gTimer.fixed_time_step       = 1.0f / (float)FRAMERATE;
    gTimer.delta_time            = gTimer.fixed_time_step;
}

INTERNAL void CapFramerate ()
{
    gTimer.end_counter = SDL_GetPerformanceCounter();
    U64 elapsed_counter = gTimer.end_counter - gTimer.last_counter;

    float elapsed = CounterToSeconds(elapsed_counter);
    while (elapsed < gTimer.fixed_time_step)
    {
        float time_left = gTimer.fixed_time_step - elapsed;
        U32 sleep = (U32)(1000.0f * time_left);
        if (sleep > 0) SDL_Delay(sleep);

        elapsed_counter = SDL_GetPerformanceCounter() - gTimer.last_counter;
        elapsed = CounterToSeconds(elapsed_counter);
    }

    gTimer.current_fps = (float)gTimer.performance_frequency / (float)elapsed_counter;
    gTimer.last_counter = SDL_GetPerformanceCounter();

    // Display the current FPS in the window title in debug builds.
    #if defined(BUILD_DEBUG)
    std::string window_title = std::string(WINDOW_TITLE) + " (FPS: " + std::to_string(gTimer.current_fps) + ")";
    SDL_SetWindowTitle(gWindow.window, window_title.c_str());
    #endif // BUILD_DEBUG
}

#endif // PLATFORM_WIN32 =======================================================

#if defined(PLATFORM_WEB) // ===================================================

INTERNAL void InitFrameTimer ()
{
    gTimer.performance_frequency = SDL_GetPerformanceFrequency();
    gTimer.last_counter          = SDL_GetPerformanceCounter();
    gTimer.end_counter           = 0;
    gTimer.current_fps           = 0.0f;
    gTimer.fixed_time_step       = 0.0f;
    gTimer.delta_time            = 0.0f;
}

INTERNAL void CapFramerate ()
{
    gTimer.end_counter = SDL_GetPerformanceCounter();
    U64 elapsed_counter = gTimer.end_counter - gTimer.last_counter;
    gTimer.last_counter = SDL_GetPerformanceCounter();

    gTimer.current_fps = (float)gTimer.performance_frequency / (float)elapsed_counter;

    gTimer.delta_time = CounterToSeconds(elapsed_counter);
    gTimer.fixed_time_step = CounterToSeconds(elapsed_counter);
}

#endif // PLATFORM_WEB =========================================================
