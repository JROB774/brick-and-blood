INTERNAL bool IsDay ()
{
    int day_turns = gGameState.turns % NIGHT_END; // Get the turn counter for the current day.
    return (day_turns < EVENING_END);
}

INTERNAL bool IsNight ()
{
    int day_turns = gGameState.turns % NIGHT_END; // Get the turn counter for the current day.
    return (day_turns >= EVENING_END && day_turns < NIGHT_END);
}

INTERNAL void StartGame ()
{
    gApplication.state = APP_STATE_GAME;

    InitMap();
    InitPlayer();

    EnableLighting(true);
    SetLightingIntensity(0);

    gGameState.turns = 0;
}

INTERNAL void UpdateGame ()
{
    // Generate a whole new world map.
    #if defined(BUILD_DEBUG)
    if (IsKeyPressed(SDL_SCANCODE_R)) InitMap();
    #endif

    int day_turns = gGameState.turns % NIGHT_END; // Get the turn counter for the current day.
    // Set the lighting based on time of day.
    if      (day_turns <     DAY_END) SetLightingIntensity(0.00f);
    else if (day_turns < EVENING_END) SetLightingIntensity(0.25f);
    else if (day_turns <   NIGHT_END) SetLightingIntensity(1.00f);

    UpdatePlayer();
    UpdateMap();
}

INTERNAL void RenderGame ()
{
    BeginLighting();
    BeginCamera();
    RenderMap();
    EndCamera();
    EndLighting();
    RenderPlayer();
}
