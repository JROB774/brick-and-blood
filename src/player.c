INTERNAL void create_player ()
{
    // Nothing...
}

INTERNAL void update_player (float dt)
{
    // Camera controls.
    float camera_speed = (key_down(KEY_LSHIFT)) ? 500.0f : 250.0f;
    if (key_down(KEY_W)) gCamera.ypos -= camera_speed * dt;
    if (key_down(KEY_S)) gCamera.ypos += camera_speed * dt;
    if (key_down(KEY_A)) gCamera.xpos -= camera_speed * dt;
    if (key_down(KEY_D)) gCamera.xpos += camera_speed * dt;
}

INTERNAL void render_player (float dt)
{
    float cursor_x = get_mouse_x(), cursor_y = get_mouse_y();
    render_bitmap(cursor_x,cursor_y, PAL_CURSOR, &SPR_CURSOR);
}
