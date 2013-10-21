#include "levelset.hpp"
#include "tileset.hpp"
#include "player.hpp"
#include "globals.hpp"

Player::Player() : texture(GetTexture("man.png")) {}

void Player::Render() const
{
    SDL_Rect s = { killed ? WIDTH : 0, 0, WIDTH, HEIGHT };
    SDL_Rect d = { int(x) * SCREEN_MUL, int(y) * SCREEN_MUL,
                   WIDTH * SCREEN_MUL, HEIGHT * SCREEN_MUL };
    auto flag = SDL_RendererFlip(
        (flip ? SDL_FLIP_VERTICAL : 0) | (left ? SDL_FLIP_HORIZONTAL : 0));

    if (killed)
        SDL_SetTextureColorMod(texture.get(), 255, 0, 0);
    else
        SDL_SetTextureColorMod(texture.get(), 100, 255, 100);

    SDL_RenderCopyEx(renderer, texture.get(), &s, &d, 0, nullptr, flag);

    if (x > Level::WIDTH*8-WIDTH && level.Neighbor(RIGHT) == level.Name())
    {
        d.x -= Level::WIDTH*8*SCREEN_MUL;
        SDL_RenderCopyEx(renderer, texture.get(), &s, &d, 0, nullptr, flag);
    }
    if (x < 0 && level.Neighbor(LEFT) == level.Name())
    {
        d.x += Level::WIDTH*8*SCREEN_MUL;
        SDL_RenderCopyEx(renderer, texture.get(), &s, &d, 0, nullptr, flag);
    }
    if (y > Level::HEIGHT*8-HEIGHT && level.Neighbor(DOWN) == level.Name())
    {
        d.y -= Level::HEIGHT*8*SCREEN_MUL;
        SDL_RenderCopyEx(renderer, texture.get(), &s, &d, 0, nullptr, flag);
    }
    if (y < 0 && level.Neighbor(UP) == level.Name())
    {
        d.y += Level::HEIGHT*8*SCREEN_MUL;
        SDL_RenderCopyEx(renderer, texture.get(), &s, &d, 0, nullptr, flag);
    }
}

bool Player::Coll(int x, int y, double& extra_mov)
{
    char c = level.Tile(x, y);
    if (c)
    {
        auto fl = level.Tileset().GetElement(c).flags;
        if (fl & Tileset::LEFT_MOVING) extra_mov = std::max(extra_mov-1, -1.0);
        if (fl & Tileset::RIGHT_MOVING) extra_mov = std::min(extra_mov+1, 1.0);
        if (fl & Tileset::KILL) Kill();
        if (fl & Tileset::SOLID) return true;
    }
    return false;
}

void Player::CheckReset(double dt)
{
    if (killed)
    {
        killed -= dt;
        if (killed <= 0)
        {
            Reset();
            return;
        }
    }
}

void Player::Reset()
{
    killed = 0;
    x = respawn_x;
    y = respawn_y;
    flip = respawn_flip;
    dir = 0;
    if (level.Name() != respawn_level)
        level = level.OwnerLevelset().LevelFromName(respawn_level);
}

void Player::Simul(double dt, bool left, bool right)
{
    if (killed) return;

    const double ACCEL_MUL = 8;
    if (!(left || right))
    {
        if (dir > 0)
            dir -= std::min(dir, dt*ACCEL_MUL);
        else
            dir += std::min(-dir, dt*ACCEL_MUL);
    }
    if (left) dir -= dt*ACCEL_MUL;
    if (right) dir += dt*ACCEL_MUL;
    if (dir > 1) dir = 1;
    if (dir < -1) dir = -1;

    if (dir < 0) this->left = true;
    if (dir > 0) this->left = false;

    y += dt * (flip ? -1 : 1) * 200;

    int sx = std::max(int(x)/8, 0);
    int ex = std::min(int(x+WIDTH-1)/8, Level::WIDTH-1);
    int cy = int(y+(flip ? 0 : HEIGHT))/8; // TODO: ha out of range, skip?

    double extra_mov = 0;
    standing = false;
    if (cy >= 0 && cy < Level::HEIGHT)
        for (int ix = sx; ix <= ex; ++ix)
            if (Coll(ix, cy, extra_mov))
            {
                y = cy*8 + (flip ? 8 : -HEIGHT);
                standing = true;
            }

    x += dt * (dir * 200 + extra_mov * 150);
    sx = std::max(int(x)/8, 0);
    ex = std::min(int(x+WIDTH-1)/8, Level::WIDTH-1);
    int sy = std::max(int(y)/8, 0);
    int ey = std::min(int(y+HEIGHT-1)/8, Level::HEIGHT-1);

    for (int iy = sy; iy <= ey; ++iy)
    {
        if (Coll(sx, iy, extra_mov)) x = sx*8+8;
        if (Coll(ex, iy, extra_mov)) x = ex*8-WIDTH;
    }

    for (auto& et: level.Entities())
        if (!(x + WIDTH < et->X() ||
              x > et->X() + et->Width() ||
              y + HEIGHT < et->Y() ||
              y > et->Y() + et->Height()))
            et->Interact(*this);

#define CHK_WRAP(c, op, var, pm, neigh)                             \
    if (c op -var/2)                                                \
    {                                                               \
        c pm##= Level::var*8;                                       \
        auto& nlevel = level.Neighbor(neigh);                       \
        if (nlevel != level.Name())                                 \
            level = level.OwnerLevelset().LevelFromName(nlevel);    \
    }
    CHK_WRAP(x, > Level::WIDTH*8,  WIDTH,  -, RIGHT);
    CHK_WRAP(x, <,                 WIDTH,  +, LEFT);
    CHK_WRAP(y, > Level::HEIGHT*8, HEIGHT, -, DOWN);
    CHK_WRAP(y, <,                 HEIGHT, +, UP);
}
