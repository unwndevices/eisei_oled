#ifndef ENUMCLASSHASH_HPP
#define ENUMCLASSHASH_HPP
#include <stdio.h>

struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

#endif // ENUMCLASSHASH_HPP
