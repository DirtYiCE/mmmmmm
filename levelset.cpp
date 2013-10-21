#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include "entity.hpp"
#include "levelset.hpp"
#include "tileset.hpp"
#include "globals.hpp"

Level::Level(Levelset& ls, const std::string& name, std::istream& in)
    : ls(&ls), name(name), tiles()
{
    GetLine(in, tileset);
    std::string line;
    GetLine(in, line);
    auto ary = Split(line);
    if (ary.size() != 4)
        throw std::runtime_error("Invalid level neighbor specifiaction");
    std::copy(ary.begin(), ary.end(), neighbors.begin());

    GetLine(in, line);
    ary = Split(line);
    if (ary.size() != 3)
        throw std::runtime_error("Invalid level color");
    color = { uint8_t(boost::lexical_cast<int>(ary[0])),
              uint8_t(boost::lexical_cast<int>(ary[1])),
              uint8_t(boost::lexical_cast<int>(ary[2])) };

    for (size_t j = 0; j < HEIGHT; ++j)
    {
        getline(in, line);
        if (!in.good()) throw std::runtime_error("level truncated");

        size_t n = std::min(size_t(line.length()), size_t(WIDTH));
        for (size_t i = 0; i < n; ++i)
            tiles[i][j] = (line[i] == ' ') ? 0 : line[i];
    }

    GetLine(in, line);
    while (line != "end")
    {
        ents.push_back(EntityFactory::Create(Split(line)));
        GetLine(in, line);
    }
}

Tileset& Level::Tileset() const
{
    return Tileset::GetTileset(tileset);
}

void Level::Simul(double dt)
{
    for (auto& ptr : ents)
        ptr->Simul(dt);
}

bool N(char c, const std::string& n)
{
    for (char x: n)
        if (x == c) return true;
    return false;
}

void Level::Render() const
{
    auto& ts = Tileset();
    for (unsigned x = 0; x < WIDTH; ++x)
        for (unsigned y = 0; y < HEIGHT; ++y)
            if (tiles[x][y])
            {
                auto& el = ts.GetElement(tiles[x][y]);
                int k = 0;
                if (el.flags & Tileset::HAS_DIRECTION)
                {
                    auto& n = el.neighbor;
                    if (x == WIDTH-1 ||                  N(tiles[x+1][y],   n)) k |= 1;
                    if (x == WIDTH-1 || y == 0        || N(tiles[x+1][y-1], n)) k |= 2;
                    if (                y == 0        || N(tiles[x]  [y-1], n)) k |= 4;
                    if (x == 0       || y == 0        || N(tiles[x-1][y-1], n)) k |= 8;
                    if (x == 0       ||                  N(tiles[x-1][y],   n)) k |= 16;
                    if (x == 0       || y == HEIGHT-1 || N(tiles[x-1][y+1], n)) k |= 32;
                    if (                y == HEIGHT-1 || N(tiles[x]  [y+1], n)) k |= 64;
                    if (x == WIDTH-1 || y == HEIGHT-1 || N(tiles[x+1][y+1], n)) k |= 128;
                }

                SDL_Rect s = { int(el.coords[k].x*8), int(el.coords[k].y*8), 8, 8 };
                SDL_Rect d = { int(x*8*SCREEN_MUL), int(y*8*SCREEN_MUL),
                               8*SCREEN_MUL, 8*SCREEN_MUL };
                if (el.flags & Tileset::COLORED)
                    SDL_SetTextureColorMod(ts.Texture(), color.r, color.g, color.b);
                else
                    SDL_SetTextureColorMod(ts.Texture(), 255, 255, 255);
                SDL_RenderCopy(renderer, ts.Texture(), &s, &d);
            }

    for (auto& ptr : ents)
        ptr->Render();
}

Levelset::Levelset(const std::string& file)
{
    std::ifstream in(file);
    std::string line;

    GetLine(in, line);
    auto ary = Split(line);
    if (ary.size() != 4) throw std::runtime_error("Invalid levelset spec");
    start_level = ary[0];
    start_x = boost::lexical_cast<int>(ary[1]);
    start_y = boost::lexical_cast<int>(ary[2]);
    start_flipped = boost::lexical_cast<bool>(ary[3]);

    while (GetLine(in, line))
    {
        levels.emplace(std::piecewise_construct,
                       std::forward_as_tuple(line),
                       std::forward_as_tuple(*this, line, in));
    }

    if (in.bad() || levels.size() == 0)
        throw std::runtime_error("Levelset: no levels loaded");
}

void Levelset::SetRespawns() const
{
    respawn_x = start_x;
    respawn_y = start_y;
    respawn_flip = start_flipped;
    respawn_level = start_level;
}
