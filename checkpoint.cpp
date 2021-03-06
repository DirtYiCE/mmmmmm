#include <boost/lexical_cast.hpp>
#include "checkpoint.hpp"
#include "levelset.hpp"
#include "player.hpp"
#include "globals.hpp"

int* Checkpoint::active_cp = nullptr;

Checkpoint::Checkpoint(const std::vector<std::string>& args)
    : EntityFrom(0, 0, WIDTH, HEIGHT), text(GetTexture("checkpoint.png"))
{
    if (args.size() < 4)
        throw std::runtime_error("checkpoint: too few arguments");

    x = boost::lexical_cast<int>(args[1]);
    y = boost::lexical_cast<int>(args[2]);
    flip = boost::lexical_cast<bool>(args[3]);

    handle = new int(1);
}

Checkpoint::Checkpoint(const Checkpoint& o)
    : EntityFrom(o), text(o.text), handle(o.handle), flip(o.flip)
{ ++*handle; }

Checkpoint::~Checkpoint()
{
    if (!--*handle)
    {
        if (active_cp == handle) active_cp = nullptr;
        delete handle;
    }
}

void Checkpoint::Render()
{
    SDL_Rect t = { int(x)*SCREEN_MUL, int(y)*SCREEN_MUL,
                   WIDTH*SCREEN_MUL, HEIGHT*SCREEN_MUL };
    if (active_cp == handle)
        SDL_SetTextureColorMod(text.get(), 255, 255, 255);
    else
        SDL_SetTextureColorMod(text.get(), 128, 128, 128);
    SDL_RenderCopyEx(renderer, text.get(), nullptr, &t, 0, nullptr,
                     flip ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE);
}

void Checkpoint::Interact(Player& p)
{
    active_cp = handle;
    respawn_x = x + (WIDTH - Player::WIDTH)/2;
    respawn_y = flip ? y : y + HEIGHT - Player::HEIGHT;
    respawn_flip = flip;
    respawn_level = level.Name();
}

static EntityFactoryImpl<Checkpoint> checkpoint_factory("checkpoint");
