INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);
    init_camera();
    set_camera_smoothing(0.1f);
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

    update_player(dt);
    update_camera(dt);
}

INTERNAL void render_application (float dt)
{
    render_clear(get_palette_color(PAL_BACKGROUND,0));

    begin_camera();
    render_map(120, 20);
    end_camera();

    render_player(dt);
}
