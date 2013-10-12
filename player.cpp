#include <SDL_image.h>
#include "levelset.hpp"
#include "tileset.hpp"
#include "player.hpp"
#include "globals.hpp"

Player::Player() : texture(IMG_LoadTexture(renderer, "man.png"))
{
    if (!texture)
        throw std::runtime_error("Failed to load man.png");
}

void Player::Render() const
{
    SDL_Rect s = { killed ? WIDTH : 0, 0, WIDTH, HEIGHT };
    SDL_Rect d = { int(x) * SCREEN_MUL, int(y) * SCREEN_MUL,
                   WIDTH * SCREEN_MUL, HEIGHT * SCREEN_MUL };
    int flag = (flip ? SDL_FLIP_VERTICAL : 0) | (left ? SDL_FLIP_HORIZONTAL : 0);

    if (killed)
        SDL_SetTextureColorMod(texture.get(), 255, 0, 0);
    else
        SDL_SetTextureColorMod(texture.get(), 100, 255, 100);
    SDL_RenderCopyEx(renderer, texture.get(), &s, &d, 0, nullptr,
                     SDL_RendererFlip(flag));
}

bool Player::Coll(const Level& level, int x, int y)
{
    char c = level.Tile(x, y);
    if (c)
    {
        auto fl = level.Tileset().GetElement(c).flags;
        if (fl & Tileset::KILL) killed = 1;
        if (fl & Tileset::SOLID) return true;
    }
    return false;
}

bool Player::NeedsReset(double dt)
{
    if (killed)
    {
        killed -= dt;
        if (killed <= 0)
        {
            killed = 0;
            return true;
        }
    }
    return false;
}

void Player::Simul(const Level& level, double dt, bool left, bool right)
{
    if (killed) return;

    int dir = 0;
    if (left) dir -= 1;
    if (right) dir += 1;

    if (dir < 0) this->left = true;
    if (dir > 0) this->left = false;

    y += dt * (flip ? -1 : 1) * 200;

    int sx = int(x)/8;
    int ex = int(x+WIDTH)/8;
    int cy = int(y+(flip ? 0 : HEIGHT))/8;

    standing = false;
    for (int ix = sx; ix <= ex; ++ix)
        if (Coll(level, ix, cy))
        {
            y = cy*8 + (flip ? 8 : -HEIGHT);
            standing = true;
            break;
        }

    x += dt * dir * 200;
    sx = int(x)/8;
    ex = int(x+WIDTH)/8;
    int sy = int(y)/8;
    int ey = int(y+HEIGHT-1)/8;

    for (int iy = sy; iy <= ey; ++iy)
    {
        if (Coll(level, sx, iy)) x = sx*8+8;
        if (Coll(level, ex, iy)) x = ex*8-WIDTH-1;
    }

}
