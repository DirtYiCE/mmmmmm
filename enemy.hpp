#ifndef UUID_6821FAFD_728F_4874_8D59_FDBD96F42188
#define UUID_6821FAFD_728F_4874_8D59_FDBD96F42188
#pragma once

#include <SDL.h>
#include "entity.hpp"

class Enemy : public EntityFrom<Enemy>
{
public:
    Enemy(const std::vector<std::string>& args);

    void Simul(double dt) override;
    void Render() override;
    void Interact(Player& p) override;

private:
    bool horiz, posdir = true;
    int min, max;
    std::shared_ptr<SDL_Texture> text;
};
#endif
