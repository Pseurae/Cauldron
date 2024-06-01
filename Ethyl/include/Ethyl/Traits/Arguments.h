#ifndef ETHYL_TRAITS_ARGUMENTS_H
#define ETHYL_TRAITS_ARGUMENTS_H

namespace Ethyl::Traits
{
template<typename... Args>
struct Arguments
{
    enum { Arity = sizeof...(Args) };  
};
}

#endif // ETHYL_TRAITS_ARGUMENTS_H