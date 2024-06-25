#ifndef ETHYL_TRAITS_CONSTRUCTIBLE_H
#define ETHYL_TRAITS_CONSTRUCTIBLE_H

namespace Ethyl::Traits
{
template<typename T, typename... Args>
inline constexpr bool Constructible = (std::is_aggregate_v<T> ? requires(Args&&... args) { T{std::forward<Args>(args)...}; } : std::constructible_from<T, Args...>);
}

#endif // ETHYL_TRAITS_CONSTRUCTIBLE_H