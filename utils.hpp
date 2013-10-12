#ifndef UUID_10520266_F975_4DAE_BCB3_B3360D4F0225
#define UUID_10520266_F975_4DAE_BCB3_B3360D4F0225
#pragma once

#include <iostream>
#include "sdl_ptr.hpp"

bool GetLine(std::istream& in, std::string& str);
std::shared_ptr<SDL_Texture> GetTexture(const std::string& str);

#endif
