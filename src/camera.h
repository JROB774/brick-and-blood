#ifndef CAMERA_H
#define CAMERA_H

GLOBAL struct
{
    float xpos;
    float ypos;

    // Used when camera smoothing is enabled.
    float smooth_xpos;
    float smooth_ypos;
    float invert_smooth_amount;

    // Shake data.
    float shake_time;
    float shake_x;
    float shake_y;
    float shake_xoff;
    float shake_yoff;

} gCamera;

INTERNAL void init_camera ();

// Wrap a block of rendering code in these calls for the rendering to be in camera/view space.
INTERNAL void begin_camera ();
INTERNAL void end_camera ();

INTERNAL void set_camera_smoothing (float smooth_amount); // [0,1]

INTERNAL void update_camera (float dt);

INTERNAL void shake_camera (float x, float y, float duration); // [-x,x] and [-y,y]

#endif /* CAMERA_H */
