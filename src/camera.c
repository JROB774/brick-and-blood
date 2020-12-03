INTERNAL void begin_camera ()
{
    gRenderer.xoffset = -CAST(int,gCamera.xpos + gCamera.shake_xoff);
    gRenderer.yoffset = -CAST(int,gCamera.ypos + gCamera.shake_yoff);
}

INTERNAL void end_camera ()
{
    gRenderer.xoffset = 0;
    gRenderer.yoffset = 0;
}

INTERNAL void update_camera (float dt)
{
    // Handle shaking the camera if necessary.
    if (gCamera.shake_time > 0.0f)
    {
        gCamera.shake_xoff = random_float_range(-gCamera.shake_x, gCamera.shake_x);
        gCamera.shake_yoff = random_float_range(-gCamera.shake_y, gCamera.shake_y);
        gCamera.shake_time -= dt;
        if (gCamera.shake_time <= 0.0f)
        {
            gCamera.shake_xoff = 0.0f;
            gCamera.shake_yoff = 0.0f;
        }
    }
}

INTERNAL void shake_camera (float x, float y, float duration)
{
    gCamera.shake_x = fabs(x), gCamera.shake_y = fabs(y);
    gCamera.shake_time = duration;
}
