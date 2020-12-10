#ifndef MAIN_WIN32_HPP
#define MAIN_WIN32_HPP

#define PLATFORM_WIN32

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <dbghelp.h>

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <ctime>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>
#include <map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_mixer.h>

#include <GON.hpp>
#include <GON.cpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

// Header includes.
#include "utility.hpp"
#include "error.hpp"
#include "image.hpp"
#include "font.hpp"
#include "settings.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "timer.hpp"
#include "input.hpp"
#include "mixer.hpp"
#include "animation.hpp"
#include "particle.hpp"
#include "collision.hpp"
#include "asset_manager.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "game.hpp"
#include "application.hpp"

// Source includes.
#include "utility.cpp"
#include "error.cpp"
#include "image.cpp"
#include "font.cpp"
#include "settings.cpp"
#include "window.cpp"
#include "renderer.cpp"
#include "timer.cpp"
#include "input.cpp"
#include "mixer.cpp"
#include "animation.cpp"
#include "particle.cpp"
#include "collision.cpp"
#include "asset_manager.cpp"
#include "camera.cpp"
#include "player.cpp"
#include "game.cpp"
#include "application.cpp"

#endif /* MAIN_WIN32_HPP */
