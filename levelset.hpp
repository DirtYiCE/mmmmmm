#ifndef UUID_2167BEB9_39F5_415E_909B_16F1342EEB4E
#define UUID_2167BEB9_39F5_415E_909B_16F1342EEB4E
#pragma once

#include <map>
#include <iostream>

class Level
{
public:
    Level(std::istream& in);

    void Render() const;

private:
    std::string tileset;
    char tiles[20][15];
};

class Levelset
{
public:
    Levelset(const std::string& file);

    const Level& StartLevel() { return levels.at(start_level); }

private:
    std::map<std::string, Level> levels;
    std::string start_level;
};
#endif
