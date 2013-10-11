#include "getline.hpp"
#include "tileset.hpp"
#include "levelset.hpp"
#include <algorithm>
#include <fstream>

extern SDL_Renderer* renderer;
extern const int SCREEN_MUL;

Level::Level(std::istream& in) : tiles()
{
    GetLine(in, tileset);

    std::string line;
    for (size_t j = 0; j < 15; ++j)
    {
        j == 0 ? GetLine(in, line) : getline(in, line);
        size_t n = std::min(size_t(line.length()), size_t(20));
        for (size_t i = 0; i < n; ++i)
            tiles[i][j] = (line[i] == ' ') ? 0 : line[i];
    }
}

void Level::Render() const
{
    auto& ts = Tileset::GetTileset(tileset);
    for (unsigned x = 0; x < 20; ++x)
        for (unsigned y = 0; y < 15; ++y)
            if (tiles[x][y])
            {
                auto& el = ts.GetElement(tiles[x][y]);
                int k = 0;
                if (el.flags & Tileset::HAS_DIRECTION)
                {
                    char n = el.neighbor;
                    if (x == 19 ||            tiles[x+1][y]   == n) k |= 1;
                    if (x == 19 || y == 0  || tiles[x+1][y-1] == n) k |= 2;
                    if (           y == 0  || tiles[x]  [y-1] == n) k |= 4;
                    if (x == 0  || y == 0  || tiles[x-1][y-1] == n) k |= 8;
                    if (x == 0  ||            tiles[x-1][y]   == n) k |= 16;
                    if (x == 0  || y == 14 || tiles[x-1][y+1] == n) k |= 32;
                    if (           y == 14 || tiles[x]  [y+1] == n) k |= 64;
                    if (x == 19 || y == 14 || tiles[x+1][y+1] == n) k |= 128;
                }

                SDL_Rect s = { int(el.coords[k][0]*16), int(el.coords[k][1]*16), 16, 16 };
                SDL_Rect d = { int(x*16*SCREEN_MUL), int(y*16*SCREEN_MUL),
                               16*SCREEN_MUL, 16*SCREEN_MUL };
                SDL_RenderCopy(renderer, ts.Texture(), &s, &d);
            }
}

Levelset::Levelset(const std::string& file)
{
    std::ifstream in(file);
    GetLine(in, start_level);

    std::string line;
    while (GetLine(in, line))
    {
        levels.emplace(std::piecewise_construct,
                       std::forward_as_tuple(line),
                       std::forward_as_tuple(in));
    }

    if (in.bad() || levels.size() == 0)
        throw std::runtime_error("Levelset: no levels loaded");
}
