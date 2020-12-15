INTERNAL void UpdatePlayer ()
{
    gPlayer.update = false;

    // If any of the player's input keys are pressed we update the simulation.
    if (IsKeyPressed(SDL_SCANCODE_A) ||
        IsKeyPressed(SDL_SCANCODE_D) ||
        IsKeyPressed(SDL_SCANCODE_W) ||
        IsKeyPressed(SDL_SCANCODE_S) ||
        IsKeyPressed(SDL_SCANCODE_SPACE))
    {
        gPlayer.update = true;
    }

    // The camera tracks the player's current position!
    Entity* p = MapGetFirstEntityOfType("player");
    if (p)
    {
        float cx = roundf(p->draw.pos.x + (TILE_W/2) - (WINDOW_SCREEN_W/2));
        float cy = roundf(p->draw.pos.y + (TILE_H/2) - (WINDOW_SCREEN_H/2));
        SetCameraTarget(cx,cy);
    }
}
