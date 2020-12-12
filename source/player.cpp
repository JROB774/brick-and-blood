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
}
