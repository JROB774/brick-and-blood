#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#define APP_VERSION_MAJOR 1
#define APP_VERSION_MINOR 0
#define APP_VERSION_PATCH 0

GLOBAL struct ApplicationState
{

} gAppState;

INTERNAL void InitApplication   ();
INTERNAL void QuitApplication   ();
INTERNAL void UpdateApplication (float dt);
INTERNAL void RenderApplication (float dt);

#endif /* APPLICATION_HPP */
