#ifndef UUID_66F74ED5_D511_4EDC_8D4E_FC4F6112031A
#define UUID_66F74ED5_D511_4EDC_8D4E_FC4F6112031A
#pragma once

#include <map>
#include "sdl_ptr.hpp"
#include "utils.hpp"

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
        COLORED       = 1 << 3,
        H_FLIP        = 1 << 4,
        V_FLIP        = 1 << 5,
        LEFT_MOVING   = 1 << 6,
        RIGHT_MOVING  = 1 << 7,
    };
    struct Element
    {
        Flags flags;
        std::array<Point, 256> coords;
        std::string neighbor;
        int anim_length;
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
