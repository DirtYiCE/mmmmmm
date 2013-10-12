#ifndef UUID_66F74ED5_D511_4EDC_8D4E_FC4F6112031A
#define UUID_66F74ED5_D511_4EDC_8D4E_FC4F6112031A
#pragma once

#include <map>
#include "sdl_ptr.hpp"

class Tileset
{
public:
    Tileset(const std::string& file);

    static Tileset& GetTileset(const std::string& file);

    enum Flags
    {
        HAS_DIRECTION = 1 << 0,
        KILL          = 1 << 1,
        SOLID         = 1 << 2,
    };
    struct Element
    {
        Flags flags;
        unsigned coords[256][2];
        char neighbor;
    };

    const Element& GetElement(char c) const
    { return elements.at(c); }

    SDL_Texture* Texture() { return text.get(); }
private:
    void LoadElements(const std::string& file);

    std::map<char, Element> elements;

    std::shared_ptr<SDL_Texture> text;
};
#endif
