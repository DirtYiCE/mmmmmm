#include <boost/lexical_cast.hpp>
#include "checkpoint.hpp"
#include "player.hpp"
#include "utils.hpp"
#include "globals.hpp"

Checkpoint* Checkpoint::active_cp = nullptr;

Checkpoint::Checkpoint(const std::vector<std::string>& args)
    : EntityFrom(0, 0, WIDTH, HEIGHT), text(GetTexture("checkpoint.png"))
{
    if (args.size() < 3)
        throw std::runtime_error("checkpoint: too few arguments");

    x = boost::lexical_cast<int>(args[1]);
    y = boost::lexical_cast<int>(args[2]);
}

Checkpoint::~Checkpoint()
{
    if (active_cp == this) active_cp = nullptr;
}

void Checkpoint::Render()
{
    SDL_Rect t = { x*SCREEN_MUL, y*SCREEN_MUL,
                   WIDTH*SCREEN_MUL, HEIGHT*SCREEN_MUL };
    if (active_cp == this)
        SDL_SetTextureColorMod(text.get(), 255, 255, 255);
    else
        SDL_SetTextureColorMod(text.get(), 128, 128, 128);
    SDL_RenderCopy(renderer, text.get(), nullptr, &t);
}

void Checkpoint::Interact(Player& p)
{
    active_cp = this;
    respawn_x = x + (WIDTH - Player::WIDTH)/2;
    respawn_y = y + HEIGHT - Player::HEIGHT;
    respawn_flip = false;
}

static EntityFactoryImpl<Checkpoint> checkpoint_factory("checkpoint");
