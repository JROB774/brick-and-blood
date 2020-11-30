INTERNAL bool init_application ()
{
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
    update_camera(dt);
}

INTERNAL void render_application (float dt)
{
    render_clear(get_palette_color(PAL_BACKGROUND,0));
    render_map(120, 20);
}
