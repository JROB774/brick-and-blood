#include "main_win32.hpp"

void QuitSystems ()
{
    QuitApplication();
    QuitMixer();
    QuitInput();
    QuitWindow();
    QuitErrorSystem();
}

int main (int argc, char** argv)
{
    ErrorTerminateCallback = QuitSystems;

    gWindow.running = true;

    InitErrorSystem();
    LoadSettings();
    InitWindow();
    InitMixer();
    InitFrameTimer();

    RandomSeed();
    InitApplication();
    ShowWindow();

    while (gWindow.running)
    {
        UpdateInputState();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            HandleInputEvents(event);
            switch (event.type)
            {
                case (SDL_QUIT):
                {
                    gWindow.running = false;
                } break;
            }
        }

        if (IsKeyPressed(SDL_SCANCODE_F) || IsKeyPressed(SDL_SCANCODE_F11) ||
            (IsKeyDown(SDL_SCANCODE_LALT) && IsKeyPressed(SDL_SCANCODE_RETURN)))
        {
            SetFullscreen(!IsFullscreen());
        }

        ClearWindow({ 0,0,0,1 });
        SetViewport();
        UpdateApplication(gTimer.delta_time);
        RenderApplication(gTimer.delta_time);
        UnsetViewport();

        CapFramerate();

        RefreshWindow();
    }

    SaveSettings();

    QuitSystems();

    return 0;
}
