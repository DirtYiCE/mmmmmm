#ifndef UUID_BC9DBCCD_E6BE_4CD0_8844_6B25FD1F2B34
#define UUID_BC9DBCCD_E6BE_4CD0_8844_6B25FD1F2B34
#pragma once

#include <SDL.h>
#include <string>

extern const int SCREEN_MUL;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern int respawn_x, respawn_y;
extern bool respawn_flip;
extern std::string respawn_level;

class Level;
extern Level level;

#endif
