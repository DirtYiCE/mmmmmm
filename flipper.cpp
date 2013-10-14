#include <boost/lexical_cast.hpp>
#include <SDL.h>
#include "flipper.hpp"
#include "player.hpp"
#include "globals.hpp"

Flipper::Flipper(const std::vector<std::string>& args)
{
    if (args.size() != 4)
        throw std::runtime_error("Invalid flipper definition");
    x = boost::lexical_cast<int>(args[1]);
    y = boost::lexical_cast<int>(args[2]);
    int l = boost::lexical_cast<int>(args[3]);
    if (args[0] == "hflipper")
    {
        width = l;
        height = 1;
    }
    else
    {
        width = 1;
        height = l;
    }
}

void Flipper::Simul(double)
{
    if (interacted) --interacted;
}

void Flipper::Render()
{
    SDL_Rect r = { int(x)*SCREEN_MUL, int(y)*SCREEN_MUL,
                   width*SCREEN_MUL, height*SCREEN_MUL };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Flipper::Interact(Player& p)
{
    if (!interacted)
        p.Flipped(!p.Flipped());
    interacted = 2;
}

static EntityFactoryImpl<Flipper> hflip_fact("hflipper");
static EntityFactoryImpl<Flipper> vflip_fact("vflipper");
