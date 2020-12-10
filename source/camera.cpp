INTERNAL void CreateCamera (Camera& camera, float left, float right, float bottom, float top)
{
    camera.projection_matrix = glm::ortho(left,right,bottom,top);
    camera.view_matrix = Mat4(1.0f);

    camera.x = 0.0f, camera.current_x = 0.0f;
    camera.y = 0.0f, camera.current_y = 0.0f;

    camera.l = left;
    camera.r = right;
    camera.b = bottom;
    camera.t = top;
}

INTERNAL void UpdateCamera (Camera& camera)
{
    camera.current_x = Lerp(camera.current_x, camera.x, gApplication.delta_time * 5);
    camera.current_y = Lerp(camera.current_y, camera.y, gApplication.delta_time * 5);

    camera.view_matrix = glm::translate(Mat4(1.0f), Vec3(camera.x,camera.y,0.0f));
}
