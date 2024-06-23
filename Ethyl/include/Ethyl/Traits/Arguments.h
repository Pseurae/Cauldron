#ifndef ETHYL_TRAITS_ARGUMENTS_H
#define ETHYL_TRAITS_ARGUMENTS_H

#include "Ethyl/Traits/Sanitize.h"

namespace Ethyl::Traits
{
template<typename... Args>
struct Arguments
{
    template<typename... Args1>
    struct Holder 
    {
        template<std::size_t I>
        using NthType = typename std::tuple_element<I, std::tuple<Args1...>>::type;
    };

    enum { Arity = sizeof...(Args) };
    using Types = Holder<Args...>;
    using SanitizedTypes = Holder<Sanitize<Args>...>;
};
}

#endif // ETHYL_TRAITS_ARGUMENTS_H