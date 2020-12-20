#pragma once

GLOBAL constexpr const char* WINDOW_TITLE    = "SANTAJAM";
GLOBAL constexpr int         WINDOW_SCREEN_W = 320;
GLOBAL constexpr int         WINDOW_SCREEN_H = 240;
GLOBAL constexpr int         WINDOW_START_W  = WINDOW_SCREEN_W*2;
GLOBAL constexpr int         WINDOW_START_H  = WINDOW_SCREEN_H*2;

GLOBAL struct Window
{
    SDL_Renderer* renderer;
    SDL_Window*   window;
    int           width;  // Used to cache size when in fullscreen!!!
    int           height; // Used to cache size when in fullscreen!!!
    bool          fullscreen;
    bool          running;

} gWindow;

INTERNAL bool InitWindow            ();
INTERNAL void QuitWindow            ();
INTERNAL void ClearWindow           (Vec4 color);
INTERNAL void RefreshWindow         ();
INTERNAL void ShowWindow            ();
INTERNAL void HideWindow            ();
INTERNAL void SetViewport           ();
INTERNAL void UnsetViewport         ();
INTERNAL void SetFullscreen         (bool enable);
INTERNAL bool IsFullscreen          ();
INTERNAL void SetWindowSize         (int width, int height);
INTERNAL int  GetWindowWidth        ();
INTERNAL int  GetWindowHeight       ();
INTERNAL int  GetCachedWindowWidth  ();
INTERNAL int  GetCachedWindowHeight ();
INTERNAL Rect GetViewport           ();
