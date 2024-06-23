#ifndef ETHYL_TRAITS_SANTIZE_H
#define ETHYL_TRAITS_SANTIZE_H

#include <type_traits>

namespace Ethyl::Traits
{
template<typename T>
using Sanitize = std::remove_pointer_t<std::remove_cvref_t<T>>;
}

#endif // ETHYL_TRAITS_SANTIZE_H