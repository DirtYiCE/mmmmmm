#ifndef UUID_0EB3A7CE_9637_47F1_86DE_4E25EC7D1A7B
#define UUID_0EB3A7CE_9637_47F1_86DE_4E25EC7D1A7B
#pragma once

#include "entity.hpp"
#include "sdl_ptr.hpp"

class Checkpoint : public EntityFrom<Checkpoint>
{
public:
    static const int WIDTH = 16;
    static const int HEIGHT = 16;

    Checkpoint(const std::vector<std::string>& args);
    Checkpoint(const Checkpoint& o);
    ~Checkpoint();
    void operator=(const Checkpoint&) = delete;

    void Render() override;
    void Interact(Player& p) override;

private:
    std::shared_ptr<SDL_Texture> text;
    int* handle;
    static int* active_cp;
    bool flip;
};

#endif
