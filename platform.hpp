#ifndef UUID_14B21E4F_BCCA_45AF_B835_67FC50D438E3
#define UUID_14B21E4F_BCCA_45AF_B835_67FC50D438E3
#pragma once

#include "entity.hpp"

class Platform : public EntityFrom<Platform>
{
public:
    Platform(const std::vector<std::string>& args);

    void Simul(double dt) override;
    void Render() override;
    void Interact(Player& p) override;

private:
    bool horiz, posdir = true;
    int min, max;
    double mov;
};

#endif
