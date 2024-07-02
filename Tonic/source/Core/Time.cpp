#include "Tonic/Core/Time.h"
#include <chrono>

namespace Tonic::Core
{
double Time::GetTickCount()
{
    std::chrono::duration<double> duration = std::chrono::system_clock::now().time_since_epoch();
    return duration.count();
}
}