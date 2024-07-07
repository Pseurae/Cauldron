#ifndef POTION_ECS_CONSTANTS_H
#define POTION_ECS_CONSTANTS_H

#include <cstddef>

namespace Potion::ECS
{
using EntityID = ::std::uintmax_t;

namespace Constants
{
    static constexpr auto MaxComponentTypes = 256;
}
}

#endif // POTION_ECS_CONSTANTS_H