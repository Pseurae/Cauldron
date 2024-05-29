#ifndef ETHYL_TRAITS_FUNCTION_H
#define ETHYL_TRAITS_FUNCTION_H

namespace Ethyl::Traits
{
template<typename Fn> 
struct Function : Function<decltype(&Fn::operator())>
{
    static constexpr bool IsFunctor = true;
};

template<typename... Args>
struct Function<void(*)(Args...)>
{
    static constexpr bool IsFunctor = false;
    enum { Arity = sizeof...(Args) };
    using Class = void;
};

template<typename Ret, typename T, typename... Args>
struct Function<Ret(T::*)(Args...)>
{
    static constexpr bool IsFunctor = false;
    enum { Arity = sizeof...(Args) };
    using Class = T;
};

template<typename Ret, typename T, typename... Args>
struct Function<Ret(T::*)(Args...) const>
{
    static constexpr bool IsFunctor = false;
    enum { Arity = sizeof...(Args) };
    using Class = T;
};
}

#endif // ETHYL_TRAITS_FUNCTION_H