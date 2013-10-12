#include <SDL.h>
#include <iostream>
#include "boost_format.hpp"
#include "sdl_ptr.hpp"
#include "levelset.hpp"
#include "player.hpp"

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

        RendererPtr ren(SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
        if (!ren) SDL_ERROR("creating renderer");
        renderer = ren.get();

        auto keys = SDL_GetKeyboardState(nullptr);

        Levelset ls(argv[1]);
        const Level* l = &ls.StartLevel();
        Player p;
        p.SetPos(ls.StartX(), ls.StartY());
        p.Flipped(ls.StartFlipped());

        bool run = true;
        auto start = SDL_GetPerformanceCounter();
        while (run)
        {
            SDL_Event ev;
            while (SDL_PollEvent(&ev))
            {
                if ((ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_CLOSE) ||
                    (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE))
                    run = false;
                else if (ev.type == SDL_KEYDOWN)
                    switch (ev.key.keysym.scancode)
                    {
                    case SDL_SCANCODE_SPACE:
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_UP:
                    case SDL_SCANCODE_DOWN:
                        p.Flip();
                        break;

                    default: ;//stfu
                    }
            }

            auto now = SDL_GetPerformanceCounter();
            double dt = double(now-start) / SDL_GetPerformanceFrequency();
            start = now;

            if (p.NeedsReset(dt))
            {
                l = &ls.StartLevel();
                p.SetPos(ls.StartX(), ls.StartY());
                p.Flipped(ls.StartFlipped());
            }

            p.Simul(*l, dt,
                    keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT],
                    keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);

            SDL_RenderClear(renderer);
            l->Render();
            p.Render();
            SDL_RenderPresent(renderer);
        }
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
