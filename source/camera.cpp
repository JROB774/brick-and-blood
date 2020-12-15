INTERNAL void UpdateCamera ()
{
    if (gCamera.timer > 0.0f)
    {
        gCamera.timer -= gApplication.delta_time;
        if (gCamera.timer <= 0.0f)
        {
            gCamera.shake_minx = 0.0f;
            gCamera.shake_miny = 0.0f;
            gCamera.shake_maxx = 0.0f;
            gCamera.shake_maxy = 0.0f;
        }
    }

    gCamera.x = Lerp(gCamera.x, gCamera.target_x, gApplication.delta_time*5);
    gCamera.y = Lerp(gCamera.y, gCamera.target_y, gApplication.delta_time*5);
}

INTERNAL void SetCameraTarget (float x, float y)
{
    gCamera.target_x = roundf(x);
    gCamera.target_y = roundf(y);
}

INTERNAL void SetCamera (float x, float y)
{
    gCamera.x        = roundf(x);
    gCamera.y        = roundf(y);
    gCamera.target_x = roundf(x);
    gCamera.target_y = roundf(y);
}

INTERNAL void BeginCamera ()
{
    gRenderOffset.x = gCamera.x;
    gRenderOffset.y = gCamera.y;

    if (gCamera.timer)
    {
        gRenderOffset.x += RandomFloatRange(gCamera.shake_minx, gCamera.shake_maxx);
        gRenderOffset.y += RandomFloatRange(gCamera.shake_miny, gCamera.shake_maxy);
    }
}

INTERNAL void EndCamera ()
{
    gRenderOffset.x = 0;
    gRenderOffset.y = 0;
}

INTERNAL void ShakeCamera (float minx, float miny, float maxx, float maxy, float duration)
{
    gCamera.shake_minx += minx;
    gCamera.shake_miny += miny;
    gCamera.shake_maxx += maxx;
    gCamera.shake_maxy += maxy;

    gCamera.timer = duration;
}

INTERNAL void ResetCameraShake ()
{
    gCamera.shake_minx = 0.0f;
    gCamera.shake_miny = 0.0f;
    gCamera.shake_maxx = 0.0f;
    gCamera.shake_maxy = 0.0f;

    gCamera.timer = 0.0f;
}
