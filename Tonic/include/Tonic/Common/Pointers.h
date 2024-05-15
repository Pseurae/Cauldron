#ifndef TONIC_COMMON_POINTER_H
#define TONIC_COMMON_POINTER_H

#include <memory>

namespace Tonic
{
template<typename T, typename Deleter = ::std::default_delete<T>>
using Unique = ::std::unique_ptr<T, Deleter>;

template<typename T>
using Shared = ::std::shared_ptr<T>;

template<typename T>
using WeakRef = ::std::weak_ptr<T>;

template<typename T, typename ... Args>
constexpr Unique<T> CreateUnique(Args&& ... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr Shared<T> CreateShared(Args&& ... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
}

#endif // TONIC_COMMON_POINTER_H