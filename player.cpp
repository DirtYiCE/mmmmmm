#include <SDL_image.h>
#include "player.hpp"
#include "globals.hpp"

Player::Player() : texture(IMG_LoadTexture(renderer, "man.png"))
{
    if (!texture)
        throw std::runtime_error("Failed to load man.png");
}

void Player::Render() const
{
    SDL_Rect s = { 0, 0, WIDTH, HEIGHT };
    SDL_Rect d = { x * SCREEN_MUL, y * SCREEN_MUL,
                   WIDTH * SCREEN_MUL, HEIGHT * SCREEN_MUL };
    int flag = (flip ? SDL_FLIP_VERTICAL : 0) | (left ? SDL_FLIP_HORIZONTAL : 0);

    SDL_SetTextureColorMod(texture.get(), 100, 255, 100);
    SDL_RenderCopyEx(renderer, texture.get(), &s, &d, 0, nullptr,
                     SDL_RendererFlip(flag));
}
