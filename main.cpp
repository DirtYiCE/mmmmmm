#include <SDL.h>
#include <iostream>
#include "boost_format.hpp"
#include "sdl_ptr.hpp"
#include "levelset.hpp"
#include "player.hpp"

#define SDL_ERROR(x)                            \
    throw std::runtime_error(boost_format(      \
        "SDL error while %1%, error: %2%", x, SDL_GetError()))

extern const int SCREEN_MUL = 2; // 1: 320x200, 2: 640x400, 3: 960x600, 4: 1280x800

SDL_Window* window;
SDL_Renderer* renderer;
int respawn_x, respawn_y;
bool respawn_flip;
std::string respawn_level;
Level level;

struct Star
{
    double x, y;
    double v;
    double i;
};
std::array<Star, 80> stars;

static void AudioCb(void *, Uint8 *stream, int len)
{
    static int start = 0;
    for (int i = 0; i < len; ++i)
    {
        int t = i+start;
        //stream[i] = ((7&(((t>>17)+1)>>2)+((t>>10)&1+2*(t>>18&1))*(("23468643"[7&t>>12]-48)+(3&t>>11))+((3&t>>17)>0)*(3&t>>9)*!(1&t>>10)*(((2+t>>10&3)^(2+t>>11&3))))*t*"@06+"[3&t>>15]/32);
        //stream[i] = t>>6^t>>8|t>>12|t&63;
        //stream[i] = t*(t>>9|t>>13);
        //stream[i] = t*(42&t>>10);

        //stream[i] = (t*3*(42&t>>10))^(t*7&t*7>>8)^(t>>12)^(t&63);
        stream[i] = (t>>7&t>>4)^((t>>12)&(t*(42&t>>10)));
    }
    start += len;
}

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

        SDL_AudioSpec spec;
        spec.freq = 8000;
        //spec.format = AUDIO_S16LSB;
        spec.format = AUDIO_U8;
        spec.channels = 1;
        spec.samples = 4096;
        spec.callback = AudioCb;
        SDL_OpenAudio(&spec, nullptr);
        SDL_PauseAudio(0);

        RendererPtr ren(SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
        if (!ren) SDL_ERROR("creating renderer");
        renderer = ren.get();

        auto keys = SDL_GetKeyboardState(nullptr);

        srand(time(nullptr));
        for (Star& s: stars)
        {
            s.x = rand() % 320;
            s.y = rand() % 200;
            s.v = 150 * (1 + double(rand()) / RAND_MAX);
            s.i = 200 * (1 + double(rand()) / RAND_MAX);
        }

        Levelset ls(argv[1]);
        ls.SetRespawns();
        level = ls.StartLevel();
        Player p;
        p.Reset();

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
            double dts = double(now-start) / SDL_GetPerformanceFrequency();
            start = now;

            for (double xd = dts; xd > 0; xd -= .01)
            {
                double dt = std::min(.01, xd);
                p.CheckReset(dt);
                if (!p.Killed()) level.Simul(dt);
                p.Simul(dt,
                        keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT],
                        keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for (Star& s: stars)
            {
                s.x -= s.v * dts;
                if (s.x < 0)
                {
                    s.x = 320;
                    s.y = rand() % 200;
                    s.v = 150 * (1 + double(rand()) / RAND_MAX);
                    s.i = 200 * (1 + double(rand()) / RAND_MAX);
                }
                SDL_SetRenderDrawColor(renderer, s.i, s.i, s.i, 255);
                SDL_Rect r = { int(s.x)*SCREEN_MUL, int(s.y)*SCREEN_MUL,
                               SCREEN_MUL, SCREEN_MUL };
                SDL_RenderFillRect(renderer, &r);
            }

            level.Render();
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
