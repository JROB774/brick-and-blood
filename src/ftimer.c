INTERNAL float counter_to_seconds (U64 counter)
{
    return (CAST(float, counter) / CAST(float, gFrameTimer.performance_frequency));
}

INTERNAL void init_frame_timer ()
{
    gFrameTimer.performance_frequency = SDL_GetPerformanceFrequency();
    gFrameTimer.last_counter          = SDL_GetPerformanceCounter();
    gFrameTimer.end_counter           = 0;
    gFrameTimer.current_fps           = 0.0f;
    #if defined(PLATFORM_WIN32)
    gFrameTimer.fixed_time_step       = 1.0f / FRAMERATE;
    gFrameTimer.delta_time            = gFrameTimer.fixed_time_step;
    #elif defined(PLATFORM_WEB)
    gFrameTimer.fixed_time_step       = 0.0f;
    gFrameTimer.delta_time            = 0.0f;
    #endif
}

#if defined(PLATFORM_WIN32)
INTERNAL void cap_framerate ()
{
    gFrameTimer.end_counter = SDL_GetPerformanceCounter();
    U64 elapsed_counter = gFrameTimer.end_counter - gFrameTimer.last_counter;

    float elapsed = counter_to_seconds(elapsed_counter);
    while (elapsed < gFrameTimer.fixed_time_step)
    {
        float time_left = gFrameTimer.fixed_time_step - elapsed;
        U32 sleep = CAST(U32, 1000.0f * time_left);
        if (sleep > 0) SDL_Delay(sleep);

        elapsed_counter = SDL_GetPerformanceCounter() - gFrameTimer.last_counter;
        elapsed = counter_to_seconds(elapsed_counter);
    }

    gFrameTimer.current_fps = CAST(float, gFrameTimer.performance_frequency) / CAST(float, elapsed_counter);
    gFrameTimer.last_counter = SDL_GetPerformanceCounter();

    // Display the current FPS in the title bar if we're in a debug build
    #if defined(BUILD_DEBUG)
    const size_t title_buffer_size = 64;
    char title_buffer[title_buffer_size];
    snprintf(title_buffer, title_buffer_size, "%s (FPS: %.2f)", WINDOW_TITLE, gFrameTimer.current_fps);
    SDL_SetWindowTitle(gWindow.window, title_buffer);
    #endif
}
#endif

#if defined(PLATFORM_WEB)
INTERNAL void cap_framerate ()
{
    gFrameTimer.end_counter = SDL_GetPerformanceCounter();
    U64 elapsed_counter = gFrameTimer.end_counter - gFrameTimer.last_counter;
    gFrameTimer.last_counter = SDL_GetPerformanceCounter();

    gFrameTimer.current_fps = CAST(float,gFrameTimer.performance_frequency) / CAST(float,elapsed_counter);

    gFrameTimer.delta_time = counter_to_seconds(elapsed_counter);
    gFrameTimer.fixed_time_step = counter_to_seconds(elapsed_counter);
}
#endif
