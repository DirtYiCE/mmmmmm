#include <SDL_image.h>
#include <map>
#include "boost_format.hpp"
#include "utils.hpp"
#include "globals.hpp"

bool GetLine(std::istream& in, std::string& str)
{
    while (true)
    {
        getline(in, str);
        for (size_t i = 0; i < str.size(); ++i)
            if (str[i] == '#') break;
            else if (str[i] == ' ') continue;
            else return true;

        if (!in.good()) return false;
    }
}

std::shared_ptr<SDL_Texture> GetTexture(const std::string& str)
{
    static std::map<std::string, std::weak_ptr<SDL_Texture>> map;
    auto ptr = map[str].lock();
    if (ptr) return ptr;

    ptr.reset(IMG_LoadTexture(renderer, str.c_str()), TextureDeleter());
    if (!ptr)
        throw std::runtime_error(boost_format(
            "Failed to load image '%1%', error %2%", str, IMG_GetError()));
    map[str] = ptr;
    return ptr;
}
