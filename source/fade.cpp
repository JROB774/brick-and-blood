GLOBAL struct
{
    float speed;
    Vec4 current;
    Vec4 target;

} gFade;

INTERNAL void FadeFrom (Vec4 color, float speed)
{
    gFade.current = color;
    gFade.target = color;
    gFade.target.a = 0.0f;
    gFade.speed = speed;
}

INTERNAL void FadeTo (Vec4 color, float speed)
{
    gFade.target = color;
    gFade.speed = speed;
}

INTERNAL void InitFade ()
{
    gFade.target = { 0,0,0,0 };
    gFade.current = { 0,0,0,0 };
}

INTERNAL void RenderFade ()
{
    gFade.current = Lerp(gFade.current, gFade.target, gApplication.delta_time*gFade.speed);
    DrawFill({ 0,0,WINDOW_SCREEN_W,WINDOW_SCREEN_H }, gFade.current);
}
