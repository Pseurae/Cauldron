#ifndef TONIC_CORE_TIME_H
#define TONIC_CORE_TIME_H

#include <stdint.h>

namespace Tonic::Core
{
struct Time
{
    static double GetTickCount();
};
}

#endif // TONIC_CORE_TIME_H