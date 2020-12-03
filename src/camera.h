#ifndef CAMERA_H
#define CAMERA_H

GLOBAL struct
{
    float xpos;
    float ypos;

    // Shake data.
    float shake_time;
    float shake_x;
    float shake_y;
    float shake_xoff;
    float shake_yoff;

} gCamera;

// Wrap a block of rendering code in these calls for the rendering to be in camera/view space.
INTERNAL void begin_camera ();
INTERNAL void end_camera ();

INTERNAL void update_camera (float dt);

INTERNAL void shake_camera (float x, float y, float duration); // [-x,x] and [-y,y]

#endif /* CAMERA_H */
