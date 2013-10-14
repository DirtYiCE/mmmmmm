#ifndef UUID_E83397F0_8E1B_476C_B6DE_9A20FB15C718
#define UUID_E83397F0_8E1B_476C_B6DE_9A20FB15C718
#pragma once

#include "entity.hpp"

class Flipper : public EntityFrom<Flipper>
{
public:
    Flipper(const std::vector<std::string>& args);

    void Simul(double dt) override;
    void Render() override;
    void Interact(Player& p) override;

private:
    int interacted = 0;
};

#endif
