#ifndef APPLICATION_H
#define APPLICATION_H

#define APP_VERSION_MAJOR 1
#define APP_VERSION_MINOR 0
#define APP_VERSION_PATCH 0

typedef enum AppState__ { APP_STATE_NONE } AppState;
typedef enum MenuState__ { MENU_STATE_NONE } MenuState;

GLOBAL struct
{
    AppState state;
    MenuState menu_state;

    int frame;

} gApp;

INTERNAL bool   init_application ();
INTERNAL void   quit_application ();
INTERNAL void handle_application (SDL_Event* event);
INTERNAL void update_application (float dt);
INTERNAL void render_application (float dt);

#endif /* APPLICATION_H */
