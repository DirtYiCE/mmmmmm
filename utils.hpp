#ifndef UUID_10520266_F975_4DAE_BCB3_B3360D4F0225
#define UUID_10520266_F975_4DAE_BCB3_B3360D4F0225
#pragma once

#include <iostream>
#include <vector>
#include "sdl_ptr.hpp"

bool GetLine(std::istream& in, std::string& str);
std::shared_ptr<SDL_Texture> GetTexture(const std::string& str);
std::vector<std::string> Split(const std::string& str, const char* sep = " ");

struct Point
{
    int x, y;
};

struct Color
{
    uint8_t r, g, b, a;
};

#endif
