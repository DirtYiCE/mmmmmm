#ifndef UUID_1A5FF7E1_42DC_4767_B3FA_B8F3E9DEAF5F
#define UUID_1A5FF7E1_42DC_4767_B3FA_B8F3E9DEAF5F
#pragma once

#include "sdl_ptr.hpp"

class Level;

class Player
{
public:
    static const int WIDTH = 9;
    static const int HEIGHT = 20;

    Player();

    void Render() const;
    bool NeedsReset(double dt);
    void Simul(const Level& level, double dt, bool left, bool right);

    void SetPos(int x, int y) { this->x = x; this->y = y; }
    void Flip() { if (standing && !killed) flip = !flip; }
    bool Flipped() const { return flip; }
    void Flipped(bool f) { flip = f; }

private:
    bool Coll(const Level& level, int x, int y);

    std::shared_ptr<SDL_Texture> texture;
    double x = 0, y = 0;
    bool flip = false;
    bool left = false;
    double killed = 0;
    bool standing;
};

#endif
