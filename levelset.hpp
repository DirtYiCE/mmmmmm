#ifndef UUID_2167BEB9_39F5_415E_909B_16F1342EEB4E
#define UUID_2167BEB9_39F5_415E_909B_16F1342EEB4E
#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "clone_ptr.hpp"
#include "entity.hpp"

class Tileset;
class Levelset;

class Level
{
public:
    static constexpr int WIDTH = 40;
    static constexpr int HEIGHT = 25;

    Level() = default;
    Level(Levelset& ls, const std::string& name, std::istream& in);

    void Simul(double dt);
    void Render() const;
    char Tile(int x, int y) const { return tiles[x][y]; }
    class Tileset& Tileset() const;
    const std::string& Neighbor(int dir) { return neighbors.at(dir); }

    const std::vector<ClonePtr<Entity>>& Entities() const { return ents; }

    const Levelset& OwnerLevelset() const { return *ls; }
    Levelset& OwnerLevelset() { return *ls; }
    const std::string& Name() const { return name; }
private:
    Levelset* ls;
    std::string name;
    std::string tileset;
    char tiles[WIDTH][HEIGHT];
    std::array<std::string, 4> neighbors;
    std::vector<ClonePtr<Entity>> ents;
};

class Levelset
{
public:
    Levelset(const std::string& file);

    const Level& StartLevel() { return levels.at(start_level); }
    int StartX() const { return start_x; }
    int StartY() const { return start_y; }
    bool StartFlipped() const { return start_flipped; }
    void SetRespawns() const;

    const Level& LevelFromName(const std::string& name) { return levels.at(name); }

private:
    std::map<std::string, Level> levels;
    std::string start_level;
    int start_x, start_y;
    bool start_flipped;
};
#endif
