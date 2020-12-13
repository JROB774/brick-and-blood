#pragma once

INTERNAL void QuitInput ();

INTERNAL void UpdateInputState  ();
INTERNAL void HandleInputEvents (SDL_Event event);

// KEYBOARD

INTERNAL bool IsKeyDown     (SDL_Scancode code);
INTERNAL bool IsKeyUp       (SDL_Scancode code);
INTERNAL bool IsKeyPressed  (SDL_Scancode code);
INTERNAL bool IsKeyReleased (SDL_Scancode code);

// MOUSE

INTERNAL int GetMouseScrollVertical   ();
INTERNAL int GetMouseScrollHorizontal ();

INTERNAL Vec2 GetMousePos         ();
INTERNAL Vec2 GetRelativeMousePos ();

INTERNAL bool IsMouseButtonDown     (int button);
INTERNAL bool IsMouseButtonUp       (int button);
INTERNAL bool IsMouseButtonPressed  (int button);
INTERNAL bool IsMouseButtonReleased (int button);

// GAMEPAD

INTERNAL bool IsButtonDown     (SDL_GameControllerButton button);
INTERNAL bool IsButtonUp       (SDL_GameControllerButton button);
INTERNAL bool IsButtonPressed  (SDL_GameControllerButton button);
INTERNAL bool IsButtonReleased (SDL_GameControllerButton button);

INTERNAL S16 GetAxis (SDL_GameControllerAxis axis);

INTERNAL bool IsRightStickUp    ();
INTERNAL bool IsRightStickRight ();
INTERNAL bool IsRightStickDown  ();
INTERNAL bool IsRightStickLeft  ();

INTERNAL bool IsRightStickUpPressed    ();
INTERNAL bool IsRightStickRightPressed ();
INTERNAL bool IsRightStickDownPressed  ();
INTERNAL bool IsRightStickLeftPressed  ();

INTERNAL bool IsLeftStickUp    ();
INTERNAL bool IsLeftStickRight ();
INTERNAL bool IsLeftStickDown  ();
INTERNAL bool IsLeftStickLeft  ();

INTERNAL bool IsLeftStickUpPressed    ();
INTERNAL bool IsLeftStickRightPressed ();
INTERNAL bool IsLeftStickDownPressed  ();
INTERNAL bool IsLeftStickLeftPressed  ();
