#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include "getline.hpp"
#include "tileset.hpp"
#include "levelset.hpp"
#include "globals.hpp"

Level::Level(std::istream& in) : tiles()
{
    GetLine(in, tileset);

    std::string line;
    for (size_t j = 0; j < 25; ++j)
    {
        j == 0 ? GetLine(in, line) : getline(in, line);
        if (!in.good()) throw std::runtime_error("level truncated");

        size_t n = std::min(size_t(line.length()), size_t(40));
        for (size_t i = 0; i < n; ++i)
            tiles[i][j] = (line[i] == ' ') ? 0 : line[i];
    }
}

Tileset& Level::Tileset() const
{
    return Tileset::GetTileset(tileset);
}

void Level::Render() const
{
    auto& ts = Tileset();
    for (unsigned x = 0; x < 40; ++x)
        for (unsigned y = 0; y < 25; ++y)
            if (tiles[x][y])
            {
                auto& el = ts.GetElement(tiles[x][y]);
                int k = 0;
                if (el.flags & Tileset::HAS_DIRECTION)
                {
                    char n = el.neighbor;
                    if (x == 39 ||            tiles[x+1][y]   == n) k |= 1;
                    if (x == 39 || y == 0  || tiles[x+1][y-1] == n) k |= 2;
                    if (           y == 0  || tiles[x]  [y-1] == n) k |= 4;
                    if (x == 0  || y == 0  || tiles[x-1][y-1] == n) k |= 8;
                    if (x == 0  ||            tiles[x-1][y]   == n) k |= 16;
                    if (x == 0  || y == 24 || tiles[x-1][y+1] == n) k |= 32;
                    if (           y == 24 || tiles[x]  [y+1] == n) k |= 64;
                    if (x == 39 || y == 24 || tiles[x+1][y+1] == n) k |= 128;
                }

                SDL_Rect s = { int(el.coords[k][0]*8), int(el.coords[k][1]*8), 8, 8 };
                SDL_Rect d = { int(x*8*SCREEN_MUL), int(y*8*SCREEN_MUL),
                               8*SCREEN_MUL, 8*SCREEN_MUL };
                SDL_RenderCopy(renderer, ts.Texture(), &s, &d);
            }
}

Levelset::Levelset(const std::string& file)
{
    std::ifstream in(file);
    std::string line;

    GetLine(in, line);
    boost::char_separator<char> sep(" ");
    boost::tokenizer<decltype(sep)> tokens(line, sep);
    std::vector<std::string> ary(tokens.begin(), tokens.end());
    if (ary.size() != 4) throw std::runtime_error("Invalid levelset spec");
    start_level = ary[0];
    start_x = boost::lexical_cast<int>(ary[1]);
    start_y = boost::lexical_cast<int>(ary[2]);
    start_flipped = boost::lexical_cast<bool>(ary[3]);

    while (GetLine(in, line))
    {
        levels.emplace(std::piecewise_construct,
                       std::forward_as_tuple(line),
                       std::forward_as_tuple(in));
    }

    if (in.bad() || levels.size() == 0)
        throw std::runtime_error("Levelset: no levels loaded");
}
