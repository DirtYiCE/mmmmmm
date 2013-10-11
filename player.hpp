#ifndef UUID_1A5FF7E1_42DC_4767_B3FA_B8F3E9DEAF5F
#define UUID_1A5FF7E1_42DC_4767_B3FA_B8F3E9DEAF5F
#pragma once

#include "sdl_ptr.hpp"

class Player
{
public:
    static const int WIDTH = 8;
    static const int HEIGHT = 16;

    Player();

    void Render() const;

    void SetPos(int x, int y) { this->x = x; this->y = y; }
    bool Flipped() const { return flip; }
    void Flipped(bool f) { flip = f; }

private:
    TexturePtr texture;
    int x = 0, y = 0;
    bool flip = false;
    bool left = false;
};

#endif
