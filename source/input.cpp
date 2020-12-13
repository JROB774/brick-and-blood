GLOBAL constexpr S16 INPUT_GAMEPAD_STICK_DEADZONE = 12000;

GLOBAL struct InputState
{
    U8 previous_key_state[SDL_NUM_SCANCODES];
    U8 current_key_state[SDL_NUM_SCANCODES];

    struct { int x,y; } mouse_wheel;

    Vec2 mouse_pos;
    Vec2 relative_mouse_pos;

    U32 previous_mouse_button_state[SDL_BUTTON_X2+1]; // There is no SDL_BUTTON_MAX...
    U32 current_mouse_button_state[SDL_BUTTON_X2+1];

    SDL_GameController* gamepad;

    U8 previous_button_state[SDL_CONTROLLER_BUTTON_MAX];
    U8 current_button_state[SDL_CONTROLLER_BUTTON_MAX];

    S16 previous_left_stick_x, previous_left_stick_y;
    S16 current_left_stick_x, current_left_stick_y;
    S16 previous_right_stick_x, previous_right_stick_y;
    S16 current_right_stick_x, current_right_stick_y;

} gInput;

INTERNAL void RemoveGamepad ()
{
    if (gInput.gamepad)
    {
        SDL_GameControllerClose(gInput.gamepad);
        gInput.gamepad = NULL;
    }
}

INTERNAL void AddGamepad ()
{
    // Search for a plugged in gamepad and if there is one use it.
    RemoveGamepad();
    for (int i=0; i<SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            gInput.gamepad = SDL_GameControllerOpen(i);
            if (gInput.gamepad)
            {
                break;
            }
        }
    }
}

INTERNAL void QuitInput ()
{
    RemoveGamepad();
}

INTERNAL void UpdateInputState ()
{
    // Update the keyboard state.
    memcpy(gInput.previous_key_state, gInput.current_key_state, SDL_NUM_SCANCODES * sizeof(U8));
    memcpy(gInput.current_key_state, SDL_GetKeyboardState(NULL), SDL_NUM_SCANCODES * sizeof(U8));

    // Update the mouse state.
    int mousex,mousey;
    SDL_GetMouseState(&mousex,&mousey);
    gInput.mouse_pos = { (float)mousex, (float)mousey };
    SDL_GetRelativeMouseState(&mousex,&mousey);
    gInput.relative_mouse_pos = { (float)mousex, (float)mousey };

    U32 mouse = SDL_GetMouseState(NULL,NULL);
    memcpy(gInput.previous_mouse_button_state, gInput.current_mouse_button_state, sizeof(gInput.previous_mouse_button_state));
    gInput.current_mouse_button_state[SDL_BUTTON_LEFT] = mouse & SDL_BUTTON_LMASK;
    gInput.current_mouse_button_state[SDL_BUTTON_MIDDLE] = mouse & SDL_BUTTON_MMASK;
    gInput.current_mouse_button_state[SDL_BUTTON_RIGHT] = mouse & SDL_BUTTON_RMASK;
    gInput.current_mouse_button_state[SDL_BUTTON_X1] = mouse & SDL_BUTTON_X1MASK;
    gInput.current_mouse_button_state[SDL_BUTTON_X2] = mouse & SDL_BUTTON_X2MASK;

    // Update the gamepad button state.
    if (gInput.gamepad)
    {
        memcpy(gInput.previous_button_state, gInput.current_button_state, sizeof(gInput.previous_button_state));
        for (int i=0; i<SDL_CONTROLLER_BUTTON_MAX; ++i)
        {
            gInput.current_button_state[i] = SDL_GameControllerGetButton(gInput.gamepad, (SDL_GameControllerButton)i);
        }

        gInput.previous_left_stick_x = gInput.current_left_stick_x, gInput.previous_left_stick_y = gInput.current_left_stick_y;
        gInput.current_left_stick_x = GetAxis(SDL_CONTROLLER_AXIS_LEFTX), gInput.current_left_stick_y = GetAxis(SDL_CONTROLLER_AXIS_LEFTY);
        gInput.previous_right_stick_x = gInput.current_right_stick_x, gInput.previous_right_stick_y = gInput.current_right_stick_y;
        gInput.current_right_stick_x = GetAxis(SDL_CONTROLLER_AXIS_RIGHTX), gInput.current_right_stick_y = GetAxis(SDL_CONTROLLER_AXIS_RIGHTY);
    }
    else
    {
        memset(gInput.previous_button_state, 0, sizeof(gInput.previous_button_state));
        memset(gInput.current_button_state, 0, sizeof(gInput.current_button_state));

        gInput.previous_left_stick_x = 0, gInput.previous_left_stick_y = 0;
        gInput.current_left_stick_x = 0, gInput.current_left_stick_y = 0;
        gInput.previous_right_stick_x = 0, gInput.previous_right_stick_y = 0;
        gInput.current_right_stick_x = 0, gInput.current_right_stick_y = 0;
    }
}

INTERNAL void HandleInputEvents (SDL_Event event)
{
    switch (event.type)
    {
        case (SDL_CONTROLLERDEVICEADDED): AddGamepad(); break;
        case (SDL_CONTROLLERDEVICEREMOVED): RemoveGamepad(); break;

        case (SDL_MOUSEWHEEL):
        {
            gInput.mouse_wheel.x = event.wheel.x;
            gInput.mouse_wheel.y = event.wheel.y;
        } break;
    }
}

// KEYBOARD

INTERNAL bool IsKeyDown (SDL_Scancode code)
{
    if (code < 0 || code > SDL_NUM_SCANCODES) return false;
    return (gInput.current_key_state[code] != 0);
}

INTERNAL bool IsKeyUp (SDL_Scancode code)
{
    if (code < 0 || code > SDL_NUM_SCANCODES) return false;
    return (gInput.current_key_state[code] == 0);
}

INTERNAL bool IsKeyPressed (SDL_Scancode code)
{
    if (code < 0 || code > SDL_NUM_SCANCODES) return false;
    return (gInput.current_key_state[code] == 1 && gInput.previous_key_state[code] == 0);
}

INTERNAL bool IsKeyReleased (SDL_Scancode code)
{
    if (code < 0 || code > SDL_NUM_SCANCODES) return false;
    return (gInput.current_key_state[code] == 0 && gInput.previous_key_state[code] == 1);
}

// MOUSE

INTERNAL int GetMouseScrollVertical ()
{
    return gInput.mouse_wheel.y;
}

INTERNAL int GetMouseScrollHorizontal ()
{
    return gInput.mouse_wheel.x;
}

INTERNAL Vec2 GetMousePos ()
{
    return gInput.mouse_pos;
}

INTERNAL Vec2 GetRelativeMousePos ()
{
    return gInput.relative_mouse_pos;
}

INTERNAL bool IsMouseButtonDown (int button)
{
    if (button < SDL_BUTTON_LEFT || button > SDL_BUTTON_X2) return false;
    return (gInput.current_mouse_button_state[button] != 0);
}
INTERNAL bool IsMouseButtonUp (int button)
{
    if (button < SDL_BUTTON_LEFT || button > SDL_BUTTON_X2) return false;
    return (gInput.current_mouse_button_state[button] == 0);
}
INTERNAL bool IsMouseButtonPressed (int button)
{
    if (button < SDL_BUTTON_LEFT || button > SDL_BUTTON_X2) return false;
    return (gInput.current_mouse_button_state[button] == 1 && gInput.previous_mouse_button_state[button] == 0);
}
INTERNAL bool IsMouseButtonReleased (int button)
{
    if (button < SDL_BUTTON_LEFT || button > SDL_BUTTON_X2) return false;
    return (gInput.current_mouse_button_state[button] == 0 && gInput.previous_mouse_button_state[button] == 1);
}

// GAMEPAD

INTERNAL bool IsButtonDown (SDL_GameControllerButton button)
{
    if (button == SDL_CONTROLLER_BUTTON_INVALID) return false;
    if (button < 0 || button > SDL_CONTROLLER_BUTTON_MAX) return false;
    if (!gInput.gamepad) return false;
    return (gInput.current_button_state[button] != 0);
}

INTERNAL bool IsButtonUp (SDL_GameControllerButton button)
{
    if (button == SDL_CONTROLLER_BUTTON_INVALID) return false;
    if (button < 0 || button > SDL_CONTROLLER_BUTTON_MAX) return false;
    if (!gInput.gamepad) return false;
    return (gInput.current_button_state[button] == 0);
}

INTERNAL bool IsButtonPressed (SDL_GameControllerButton button)
{
    if (button == SDL_CONTROLLER_BUTTON_INVALID) return false;
    if (button < 0 || button > SDL_CONTROLLER_BUTTON_MAX) return false;
    if (!gInput.gamepad) return false;
    return (gInput.current_button_state[button] == 1 && gInput.previous_button_state[button] == 0);
}

INTERNAL bool IsButtonReleased (SDL_GameControllerButton button)
{
    if (button == SDL_CONTROLLER_BUTTON_INVALID) return false;
    if (button < 0 || button > SDL_CONTROLLER_BUTTON_MAX) return false;
    if (!gInput.gamepad) return false;
    return (gInput.current_button_state[button] == 0 && gInput.previous_button_state[button] == 1);
}

INTERNAL S16 GetAxis (SDL_GameControllerAxis axis)
{
    if (axis == SDL_CONTROLLER_AXIS_INVALID) return 0;
    if (axis < 0 || axis > SDL_CONTROLLER_AXIS_MAX) return 0;
    if (!gInput.gamepad) return 0;
    return SDL_GameControllerGetAxis(gInput.gamepad, axis);
}

INTERNAL bool IsRightStickUp ()
{
    if (!gInput.gamepad) return false;
    S16 y = GetAxis(SDL_CONTROLLER_AXIS_RIGHTY);
    return (y < -INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsRightStickRight ()
{
    if (!gInput.gamepad) return false;
    S16 x = GetAxis(SDL_CONTROLLER_AXIS_RIGHTX);
    return (x > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsRightStickDown ()
{
    if (!gInput.gamepad) return false;
    S16 y = GetAxis(SDL_CONTROLLER_AXIS_RIGHTY);
    return (y > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsRightStickLeft ()
{
    if (!gInput.gamepad) return false;
    S16 x = GetAxis(SDL_CONTROLLER_AXIS_RIGHTX);
    return (x < -INPUT_GAMEPAD_STICK_DEADZONE);
}

INTERNAL bool IsRightStickUpPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_right_stick_y >= -INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_right_stick_y < -INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsRightStickRightPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_right_stick_x <= INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_right_stick_x > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsRightStickDownPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_right_stick_y <= INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_right_stick_y > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsRightStickLeftPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_right_stick_x >= -INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_right_stick_x < -INPUT_GAMEPAD_STICK_DEADZONE);
}

INTERNAL bool IsLeftStickUp ()
{
    if (!gInput.gamepad) return false;
    S16 y = GetAxis(SDL_CONTROLLER_AXIS_LEFTY);
    return (y < -INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsLeftStickRight ()
{
    if (!gInput.gamepad) return false;
    S16 x = GetAxis(SDL_CONTROLLER_AXIS_LEFTX);
    return (x > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsLeftStickDown ()
{
    if (!gInput.gamepad) return false;
    S16 y = GetAxis(SDL_CONTROLLER_AXIS_LEFTY);
    return (y > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsLeftStickLeft ()
{
    if (!gInput.gamepad) return false;
    S16 x = GetAxis(SDL_CONTROLLER_AXIS_LEFTX);
    return (x < -INPUT_GAMEPAD_STICK_DEADZONE);
}

INTERNAL bool IsLeftStickUpPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_left_stick_y >= -INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_left_stick_y < -INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsLeftStickRightPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_left_stick_x <= INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_left_stick_x > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsLeftStickDownPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_left_stick_y <= INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_left_stick_y > INPUT_GAMEPAD_STICK_DEADZONE);
}
INTERNAL bool IsLeftStickLeftPressed ()
{
    if (!gInput.gamepad) return false;
    return (gInput.previous_left_stick_x >= -INPUT_GAMEPAD_STICK_DEADZONE &&
            gInput.current_left_stick_x < -INPUT_GAMEPAD_STICK_DEADZONE);
}
