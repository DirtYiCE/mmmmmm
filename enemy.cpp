#include <boost/lexical_cast.hpp>
#include "enemy.hpp"
#include "player.hpp"
#include "utils.hpp"
#include "globals.hpp"

Enemy::Enemy(const std::vector<std::string>& args)
{
    if (args.size() != 8)
        throw std::runtime_error("Invalid enemy definition");
    horiz = args[0] == "henemy";
    text = GetTexture(args[1]);
    x = boost::lexical_cast<int>(args[2]);
    y = boost::lexical_cast<int>(args[3]);
    width = boost::lexical_cast<int>(args[4]);
    height = boost::lexical_cast<int>(args[5]);
    min = boost::lexical_cast<int>(args[6]);
    max = boost::lexical_cast<int>(args[7]);
}

void Enemy::Simul(double dt)
{
    double& c = horiz ? x : y;
    c += (posdir ? dt : -dt) * 300;

    if (c <= min) { posdir = true; c = min; }
    if (c >= max) { posdir = false; c = max; }
}

void Enemy::Render()
{
    SDL_Rect r = { int(x)*SCREEN_MUL, int(y)*SCREEN_MUL,
                   width*SCREEN_MUL, height*SCREEN_MUL };
    SDL_RenderCopy(renderer, text.get(), nullptr, &r);
}

void Enemy::Interact(Player& p)
{
    p.Kill();
}

static EntityFactoryImpl<Enemy> henemy_fact("henemy");
static EntityFactoryImpl<Enemy> venemy_fact("venemy");
