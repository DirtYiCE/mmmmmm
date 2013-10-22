#include <boost/lexical_cast.hpp>
#include "levelset.hpp"
#include "platform.hpp"
#include "player.hpp"
#include "globals.hpp"

Platform::Platform(const std::vector<std::string>& args)
{
    if (args.size() != 8)
        throw std::runtime_error("Invalid platform definition");

    horiz = args[0] == "hplatform";
    x = boost::lexical_cast<int>(args[1]);
    y = boost::lexical_cast<int>(args[2]);
    width = boost::lexical_cast<int>(args[3]);
    height = boost::lexical_cast<int>(args[4]);
    min = boost::lexical_cast<int>(args[5]);
    max = boost::lexical_cast<int>(args[6]);
    speed = boost::lexical_cast<double>(args[7]);
}

void Platform::Simul(double dt)
{
    double& c = horiz ? x : y;
    mov = (posdir ? dt : -dt) * speed;
    c += mov;

    if (c <= min) { posdir = true; c = min; mov = 0; }
    if (c >= max) { posdir = false; c = max; mov = 0; }
}

void Platform::Render()
{
    SDL_Rect r = { int(x)*SCREEN_MUL, int(y)*SCREEN_MUL,
                   width*SCREEN_MUL, height*SCREEN_MUL };
    Color c = level.Color();
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
    SDL_RenderFillRect(renderer, &r);
}

void Platform::Interact(Player& p)
{
    double dist;
    if (p.Flipped())
        dist = y+height - p.Y();
    else
        dist = p.Y()+Player::HEIGHT - y;

    if (dist < 5)
    {
        if (p.Flipped())
            p.Y(int(y)+height);
        else
            p.Y(int(y)-Player::HEIGHT);
        p.Standing(true);
        if (horiz) p.X(p.X() + mov);
    }
}

static EntityFactoryImpl<Platform> hplat_fact("hplatform");
static EntityFactoryImpl<Platform> vplat_fact("vplatform");
