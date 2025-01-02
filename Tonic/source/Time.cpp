#include "Tonic/Time.h"
#include <chrono>

namespace Tonic
{
double Time::Get()
{
    std::chrono::duration<double> duration = std::chrono::system_clock::now().time_since_epoch();
    return duration.count();
}
}