INTERNAL void init_camera ()
{
    gCamera.invert_smooth_amount = 1.0f;
}

INTERNAL void begin_camera ()
{
    gRenderer.xoffset = -CAST(int,gCamera.smooth_xpos + gCamera.shake_xoff);
    gRenderer.yoffset = -CAST(int,gCamera.smooth_ypos + gCamera.shake_yoff);
}

INTERNAL void end_camera ()
{
    gRenderer.xoffset = 0;
    gRenderer.yoffset = 0;
}

INTERNAL void set_camera_smoothing (float smooth_amount)
{
    smooth_amount = CLAMP(smooth_amount, 0.0f,1.0f);
    if (smooth_amount <= 0.0f)
    {
        gCamera.invert_smooth_amount = 1.0f; // This will make the lerp instantly hit its target.
    }
    else
    {
        // We invert the value pased in, due to how lerp works. This makes it so that if the user
        // enters 1 as their smoothing value they get more smoothing, and lower values give less.
        // We have a special case for handling a value of 1 as otherwise we'd end up with zero.
        if (smooth_amount == 1.0f)
        {
            gCamera.invert_smooth_amount = 0.01f; // Just a low value for higher smoothing.
        }
        else
        {
            gCamera.invert_smooth_amount = (1.0f - smooth_amount) / 10.0f;
        }
    }
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

    gCamera.smooth_xpos = lerp(gCamera.smooth_xpos, gCamera.xpos, gCamera.invert_smooth_amount);
    gCamera.smooth_ypos = lerp(gCamera.smooth_ypos, gCamera.ypos, gCamera.invert_smooth_amount);
}

INTERNAL void shake_camera (float x, float y, float duration)
{
    gCamera.shake_x = fabs(x), gCamera.shake_y = fabs(y);
    gCamera.shake_time = duration;
}
