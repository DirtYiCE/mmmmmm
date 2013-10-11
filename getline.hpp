#ifndef UUID_10520266_F975_4DAE_BCB3_B3360D4F0225
#define UUID_10520266_F975_4DAE_BCB3_B3360D4F0225
#pragma once

#include <iostream>

inline bool GetLine(std::istream& in, std::string& str)
{
    while (true)
    {
        getline(in, str);
        for (size_t i = 0; i < str.size(); ++i)
            if (str[i] == '#') break;
            else if (str[i] == ' ') continue;
            else return true;

        if (!in.good()) return false;
    }
}

#endif
