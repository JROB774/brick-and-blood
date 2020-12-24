GLOBAL const std::map<char,SDL_Rect> BIG_NUMBER_CLIPS
{
{ '0', {   0,0,32,32 } },
{ '1', {  32,0,16,32 } },
{ '2', {  48,0,32,32 } },
{ '3', {  80,0,32,32 } },
{ '4', { 112,0,32,32 } },
{ '5', { 144,0,32,32 } },
{ '6', { 176,0,32,32 } },
{ '7', { 208,0,32,32 } },
{ '8', { 240,0,32,32 } },
{ '9', { 272,0,32,32 } }
};

INTERNAL void StartLose ()
{
    gApplication.state = APP_STATE_LOSE;
    gLoseState.leaving = false;
    gLoseState.cooldown = 0.25f;
}

INTERNAL void UpdateLose ()
{
    if (gLoseState.cooldown <= 0.0f)
    {
        if (IsAnyKeyPressed())
        {
            FadeTo(SDLColorToColor({ 38,13,28,255 }), 10);
            gLoseState.leaving = true;
        }
        if (gLoseState.leaving)
        {
            if (IsFadeToDone())
            {
                FadeFrom(SDLColorToColor({ 38,13,28,255 }), 10);
                StartMenu();
            }
        }
    }
    else
    {
        gLoseState.cooldown -= gApplication.delta_time;
    }
}

INTERNAL void RenderLose ()
{
    const Vec4 COLOR = SDLColorToColor({207,198,184,255 });

    DrawImage("gameover", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, COLOR);

    // Display the number of days that were survived.
    std::string number_of_days = std::to_string(gGameState.turns / NIGHT_END);
    float text_width = 0.0f; // Determine the width.
    for (auto c: number_of_days) text_width += (float)BIG_NUMBER_CLIPS.at(c).w;
    float x = (WINDOW_SCREEN_W - text_width) / 2;
    float y = 82;
    for (auto c: number_of_days)
    {
        DrawImage("bignumber", x,y, {1,1}, {0,0}, 0.0f, FLIP_NONE, COLOR, &BIG_NUMBER_CLIPS.at(c));
        x += BIG_NUMBER_CLIPS.at(c).w;
    }
}
