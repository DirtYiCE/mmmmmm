#ifndef UUID_2E21BEAD_26B1_4652_836F_213897170F7A
#define UUID_2E21BEAD_26B1_4652_836F_213897170F7A
#pragma once

#include <boost/format.hpp>

template <typename... Args>
std::string boost_format(const std::string& fmt, Args&&... args)
{
    boost::format bf(fmt);
    boost::format* x[] = { nullptr, &(bf % args)... };
    (void) x;
    return bf.str();
}

#endif
