#ifndef POTION_CORE_SERVICES_H
#define POTION_CORE_SERVICES_H

#include <Ethyl/Types/PolyVector.h>

namespace Potion::Core
{
// JIC I want to add common functionality
struct Service
{};

class Services final
{
public:
    template<typename T, typename... Args>
    inline T& Create(Args&&... args) { return m_Services.Create<T>(std::forward<Args>(args)...); }

    template<typename... T>
    [[nodiscard]] inline decltype(auto) Get() { return m_Services.Get<T...>(); }

    template<typename... T>
    inline void Remove() { m_Services.Remove<T...>(); }

    template<typename... T>
    [[nodiscard]] inline bool Has() { return m_Services.Has<T...>(); }

    template<typename... T>
    [[nodiscard]] inline bool HasAny() { return m_Services.HasAny<T...>(); }

private:
    Ethyl::Types::PolyVector<Service> m_Services;
};
}

#endif // POTION_CORE_SERVICES_H