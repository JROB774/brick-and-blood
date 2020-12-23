INTERNAL void InitMenu ()
{
    gMenuState.leaving = false;
}

INTERNAL void QuitMenu ()
{
    // Nothing...
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
            gApplication.state = APP_STATE_GAME;
        }
    }
}

INTERNAL void RenderMenu ()
{
    DrawImage("title", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({207,198,184,255 }));
    DrawImage("titleblood", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({ 230,72,46,255 }));
}
