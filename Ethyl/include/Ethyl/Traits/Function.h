#ifndef ETHYL_TRAITS_FUNCTION_H
#define ETHYL_TRAITS_FUNCTION_H

#include "Ethyl/Traits/Arguments.h"

namespace Ethyl::Traits
{
template<typename Fn> 
struct Function : Function<decltype(&Fn::operator())>
{
    static constexpr bool IsFunctor = true;
    static constexpr bool IsMemberFunction = false;
};

template<typename Ret, typename... Args>
struct Function<Ret(*)(Args...)> : Arguments<Args...>
{
    static constexpr bool IsFunctor = false;
    static constexpr bool IsMemberFunction = false;
    using Type = Ret(*)(Args...);
    using Class = void;
};

template<typename Ret, typename T, typename... Args>
struct Function<Ret(T::*)(Args...)> : Arguments<Args...>
{
    static constexpr bool IsFunctor = false;
    static constexpr bool IsMemberFunction = true;
    using Type = Ret(T::*)(Args...);
    using Class = T;
};

template<typename Ret, typename T, typename... Args>
struct Function<Ret(T::*)(Args...) const> : Arguments<Args...>
{
    static constexpr bool IsFunctor = false;
    static constexpr bool IsMemberFunction = true;
    using Type = Ret(T::*)(Args...) const;
    using Class = T;
};
}

#endif // ETHYL_TRAITS_FUNCTION_H