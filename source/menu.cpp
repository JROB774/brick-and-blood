INTERNAL void StartMenu ()
{
    gApplication.state = APP_STATE_MENU;
    gMenuState.changing = false;
}

INTERNAL void UpdateMenu ()
{
    switch (gMenuState.mode)
    {
        // TITLE
        case (MENU_MODE_TITLE):
        {
            if (IsAnyKeyPressed())
            {
                FadeTo(SDLColorToColor({ 38,13,28,255 }), 10);
                gMenuState.changing = true;
            }
            if (gMenuState.changing)
            {
                if (IsFadeToDone())
                {
                    FadeFrom(SDLColorToColor({ 38,13,28,255 }), 10);
                    gMenuState.mode = MENU_MODE_MAIN;
                    gMenuState.changing = false;
                }
            }
        } break;
        // MAIN
        case (MENU_MODE_MAIN):
        {
            // Allow the player to nagivate the main menu options.
            if (!gMenuState.changing)
            {
                if (IsKeyPressed(SDL_SCANCODE_W) || IsKeyPressed(SDL_SCANCODE_UP))
                {
                    gMenuState.option--;
                    if (gMenuState.option < 0)
                    {
                        gMenuState.option = MENU_OPTION_TOTAL-1;
                    }
                }
                if (IsKeyPressed(SDL_SCANCODE_S) || IsKeyPressed(SDL_SCANCODE_DOWN))
                {
                    gMenuState.option++;
                    if (gMenuState.option >= MENU_OPTION_TOTAL)
                    {
                        gMenuState.option = 0;
                    }
                }
            }
            // Handle the action based on the currently selected main menu option.
            if ((IsKeyPressed(SDL_SCANCODE_SPACE) || IsKeyPressed(SDL_SCANCODE_RETURN)) && !gMenuState.changing)
            {
                switch (gMenuState.option)
                {
                    // PLAY
                    case (MENU_OPTION_PLAY):
                    {
                        if (!gMenuState.changing)
                        {
                            FadeTo(SDLColorToColor({ 38,13,28,255 }), 10);
                            gMenuState.changing = true;
                        }
                    } break;
                    // HELP
                    case (MENU_OPTION_HELP):
                    {
                        gMenuState.mode = MENU_MODE_HELP;
                    } break;
                    // CREDITS
                    case (MENU_OPTION_CREDITS):
                    {
                        gMenuState.mode = MENU_MODE_CREDITS;
                    } break;
                    // EXIT
                    case (MENU_OPTION_EXIT):
                    {
                        gWindow.running = false;
                    } break;
                }
            }
            // If we're done fading then go to the game.
            if (gMenuState.option == MENU_OPTION_PLAY)
            {
                if (gMenuState.changing)
                {
                    if (IsFadeToDone())
                    {
                        FadeFrom(SDLColorToColor({ 38,13,28,255 }), 10);
                        StartGame();
                        gMenuState.changing = false;
                    }
                }
            }
        } break;
        // HELP and CREDITS
        case (MENU_MODE_HELP):
        case (MENU_MODE_CREDITS):
        {
            if (IsAnyKeyPressed())
            {
                gMenuState.mode = MENU_MODE_MAIN;
            }
        } break;
    }
}

INTERNAL void RenderMenu ()
{
    switch (gMenuState.mode)
    {
        // TITLE
        case (MENU_MODE_TITLE):
        {
            DrawImage("title", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({ 207,198,184,255 }));
            DrawImage("titleblood", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({ 230,72,46,255 }));
        } break;
        // MAIN
        case (MENU_MODE_MAIN):
        {
            Vec4 inactive_color = SDLColorToColor({ 207,198,184,255 });
            Vec4 active_color = SDLColorToColor({ 244,180,27,255 });

            Vec4    play_color = (gMenuState.option == MENU_OPTION_PLAY   ) ? active_color : inactive_color;
            Vec4    help_color = (gMenuState.option == MENU_OPTION_HELP   ) ? active_color : inactive_color;
            Vec4 credits_color = (gMenuState.option == MENU_OPTION_CREDITS) ? active_color : inactive_color;
            Vec4    exit_color = (gMenuState.option == MENU_OPTION_EXIT   ) ? active_color : inactive_color;

            DrawImage("menuplay",    0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE,    play_color);
            DrawImage("menuhelp",    0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE,    help_color);
            DrawImage("menucredits", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, credits_color);
            DrawImage("menuexit",    0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE,    exit_color);
        } break;
        // HELP
        case (MENU_MODE_HELP):
        {
            DrawImage("help", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({ 207,198,184,255 }));
        } break;
        // CREDITS
        case (MENU_MODE_CREDITS):
        {
            DrawImage("credits", 0,0, {1,1}, {0,0}, 0.0f, FLIP_NONE, SDLColorToColor({ 207,198,184,255 }));
        } break;
    }
}
