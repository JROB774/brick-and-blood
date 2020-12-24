#pragma once

INTERNAL void   InitLighting ();
INTERNAL void   QuitLighting ();
INTERNAL void  BeginLighting ();
INTERNAL void    EndLighting ();
INTERNAL void EnableLighting (bool enable);

INTERNAL void SetLightingIntensity (float intensity);
INTERNAL void DrawLight (float x, float y, float r);
