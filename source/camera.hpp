#ifndef CAMERA_HPP
#define CAMERA_HPP

struct Camera
{
    Mat4 projection_matrix;
    Mat4 view_matrix;

    // Separate values for lerping the camera.
    float x, current_x;
    float y, current_y;

    // Internals
    float l;
    float r;
    float b;
    float t;
};

INTERNAL void CreateCamera (Camera& camera, float left, float right, float bottom, float top);
INTERNAL void UpdateCamera (Camera& camera);

#endif /* CAMERA_HPP */
