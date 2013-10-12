#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include "boost_format.hpp"
#include "utils.hpp"
#include "tileset.hpp"
#include "globals.hpp"

//#define MATCH_DEBUG

Tileset& Tileset::GetTileset(const std::string& file)
{
    static std::map<std::string, Tileset> ts;
    auto it = ts.find(file);
    if (it == ts.end())
    {
        auto r = ts.emplace(std::piecewise_construct,
                            std::forward_as_tuple(file),
                            std::forward_as_tuple(file));
        return r.first->second;
    }
    else
        return it->second;
}

Tileset::Tileset(const std::string& file)
{
    text = GetTexture(file + ".png");
    LoadElements(file + ".tileset");
}

static bool XMatch(const std::string& s, int n)
{
    if (s.length() != 8)
        throw std::runtime_error(boost_format(
            "Invalid neighbor match '%1%'", s));

    for (size_t i = 0; i < 8; ++i)
        switch (s[i])
        {
        case '0':
            if (n & (1 << (7-i))) return false;
            break;
        case '1':
            if (!(n & (1 << (7-i)))) return false;
            break;
        case 'x':
            break;
        default:
            throw std::runtime_error(boost_format(
                "Invalid character in match: '%1%'", s[i]));
        }
    return true;
}

void Tileset::LoadElements(const std::string& file)
{
    std::ifstream in(file);

    std::string line;
    boost::char_separator<char> sep(" ");
    while (GetLine(in, line))
    {
        boost::tokenizer<decltype(sep)> tokens(line, sep);
        std::vector<std::string> ary(tokens.begin(), tokens.end());
        if (ary.size() < 4)
            throw std::runtime_error("Tileset line too short");
        if (ary[0].size() != 1)
            throw std::runtime_error(boost_format("Invalid tile character '%1%'", ary[0]));

        char c = ary[0][0];
        Element el;
        el.flags = Flags(boost::lexical_cast<std::underlying_type<Flags>::type>(ary[1]));
        bool dir = el.flags & Flags::HAS_DIRECTION;

        el.coords[0][0] = boost::lexical_cast<int>(ary[2]);
        el.coords[0][1] = boost::lexical_cast<int>(ary[3]);

        if (dir)
        {
            if (ary.size() < 5 || (ary.size() % 3) != 2)
                throw std::runtime_error(boost_format(
                    "Tileset line '%1%': invalid length", c));
            el.neighbor = ary[4][0];

            for (size_t i = 1; i < 256; ++i)
            {
                el.coords[i][0] = el.coords[0][0];
                el.coords[i][1] = el.coords[0][1];
            }

#ifdef MATCH_DEBUG
            int matched[256] = {0};
#endif
            for (size_t i = 5; i < ary.size(); i += 3)
            {
                int x = boost::lexical_cast<int>(ary[i+1]);
                int y = boost::lexical_cast<int>(ary[i+2]);
                for (size_t k = 0; k < 256; ++k)
                    if (XMatch(ary[i], k))
                    {
#ifdef MATCH_DEBUG
                        ++matched[k];
#endif
                        el.coords[k][0] = x;
                        el.coords[k][1] = y;
                    }
            }

#ifdef MATCH_DEBUG
            std::cout << "Matched " << c << ": ";
            for (size_t i = 0; i < 256; ++i)
            {
                if (matched[i] == 0) std::cout << "\033[33m";
                else if (matched[i] == 1) std::cout << "\033[32m";
                else std::cout << "\033[31m";
                std::cout << i << ". " << matched[i] << "\033[0m; ";
            }
            std::cout << std::endl;
#endif
        }

        elements[c] = el;
    }

    if (in.bad() || elements.size() == 0)
        throw std::runtime_error("Tileset: no elements loaded");
}
