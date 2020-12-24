INTERNAL void StartMenu ()
{
    gApplication.state = APP_STATE_MENU;
    gMenuState.leaving = false;
}

INTERNAL void UpdateMenu ()
{
    if (IsAnyKeyPressed())
    {
        FadeTo(SDLColorToColor({ 38,13,28,255 }), 10);
        gMenuState.leaving = true;
    }
    if (gMenuState.leaving)
    {
        if (IsFadeToDone())
        {
            FadeFrom(SDLColorToColor({ 38,13,28,255 }), 10);
            StartGame();
        }
    }
}

INTERNAL void RenderMenu ()
{
    DrawImage("title", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({207,198,184,255 }));
    DrawImage("titleblood", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({ 230,72,46,255 }));
}
