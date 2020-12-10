GLOBAL constexpr float PLAYER_MOVE_SPEED = 20.0f;
GLOBAL constexpr float PLAYER_TURN_ANGLE = 10.0f;
GLOBAL constexpr float PLAYER_TURN_SPEED = 15.0f;

INTERNAL void CreatePlayer (Player& player, int tilex, int tiley)
{
    player.tile.x        = tilex;
    player.tile.y        = tiley;
    player.draw_pos.x    = (float)(tilex*16);
    player.draw_pos.x    = (float)(tiley*16);
    player.current_angle = 0.0f;
    player.target_angle  = 0.0f;
}

INTERNAL void DeletePlayer (Player& player)
{
    // Nothing...
}

INTERNAL void UpdatePlayer (Player& player)
{
    if (IsKeyPressed(SDL_SCANCODE_A)) player.tile.x--;
    if (IsKeyPressed(SDL_SCANCODE_D)) player.tile.x++;
    if (IsKeyPressed(SDL_SCANCODE_W)) player.tile.y--;
    if (IsKeyPressed(SDL_SCANCODE_S)) player.tile.y++;

    float target_x = (float)player.tile.x*16;
    float target_y = (float)player.tile.y*16;

    player.draw_pos.x = Lerp(player.draw_pos.x, target_x, gApplication.delta_time*PLAYER_MOVE_SPEED);
    player.draw_pos.y = Lerp(player.draw_pos.y, target_y, gApplication.delta_time*PLAYER_MOVE_SPEED);

    // Handle rotating the player slightly when they are moving for some nice visual flair.
    if (roundf(target_x) != roundf(player.draw_pos.x))
    {
        if (roundf(target_x) > roundf(player.draw_pos.x)) player.target_angle = PLAYER_TURN_ANGLE;
        else if ((roundf(target_x) < roundf(player.draw_pos.x))) player.target_angle = -PLAYER_TURN_ANGLE;
    }
    else
    {
        player.target_angle = 0.0f;
    }

    player.current_angle = Lerp(player.current_angle, player.target_angle, gApplication.delta_time*PLAYER_TURN_SPEED);
}

INTERNAL void RenderPlayer (Player& player)
{
    DrawImage("player", player.draw_pos.x,player.draw_pos.y, player.current_angle);
}
