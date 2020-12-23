INTERNAL void InitMenu ()
{

}

INTERNAL void QuitMenu ()
{

}

INTERNAL void UpdateMenu ()
{

}

INTERNAL void RenderMenu ()
{
    DrawImage("title", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({207,198,184,255 }));
    DrawImage("titleblood", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({ 230,72,46,255 }));
}
