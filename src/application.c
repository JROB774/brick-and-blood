INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);
    seed_random();
    init_map();
    return true;
}

INTERNAL void quit_application ()
{
    quit_map();
}

INTERNAL void handle_application (SDL_Event* event)
{
    // Nothing...
}

INTERNAL void update_application (float dt)
{
    gApp.frame++;

    if (key_pressed(KEY_F5)) init_map();

    if (key_down(KEY_W)) gCamera.ypos -= 250 * dt;
    if (key_down(KEY_S)) gCamera.ypos += 250 * dt;
    if (key_down(KEY_A)) gCamera.xpos -= 250 * dt;
    if (key_down(KEY_D)) gCamera.xpos += 250 * dt;

    // update_camera(dt);
}

INTERNAL void render_application (float dt)
{
    render_clear(get_palette_color(PAL_BACKGROUND,0));

    begin_camera();
    render_map(120, 20);
    end_camera();

    float cursor_x = get_mouse_x();
    float cursor_y = get_mouse_y();

    render_bitmap(cursor_x,cursor_y, PAL_CURSOR, &SPR_CURSOR);
}
