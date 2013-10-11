#include <SDL.h>
#include <iostream>
#include "boost_format.hpp"
#include "sdl_ptr.hpp"
#include "levelset.hpp"

#define SDL_ERROR(x)                            \
    throw std::runtime_error(boost_format(      \
        "SDL error while %1%, error: %2%", x, SDL_GetError()))

extern const int SCREEN_MUL = 3; // 1: 320x200, 2: 640x400, 3: 960x600, 4: 1280x800

SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << boost_format("Usage: %1% level\n", argv[0]);
        return 1;
    }

    try
    {
        if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1)
            SDL_ERROR("initializing");
        WindowPtr win(SDL_CreateWindow(
            "mmmmmm", 0,0, 320*SCREEN_MUL, 200*SCREEN_MUL, SDL_WINDOW_SHOWN));
        if (!win) SDL_ERROR("creating window");
        window = win.get();

        RendererPtr ren(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
        if (!ren) SDL_ERROR("creating renderer");
        renderer = ren.get();

        Levelset ls(argv[1]);
        SDL_RenderClear(renderer);
        ls.StartLevel().Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(4000);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        SDL_Quit();
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!!" << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Quit();
    return 0;
}
