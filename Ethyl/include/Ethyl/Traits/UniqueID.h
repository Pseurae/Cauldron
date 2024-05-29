#ifndef ETHYL_TRAITS_UNIQUEID_H
#define ETHYL_TRAITS_UNIQUEID_H

#include <string_view>
#include <source_location>

namespace Ethyl::Traits
{
template<typename T>
struct UniqueID
{
private:
    template<typename>
    static constexpr std::string_view GetFuncName()
    {
        return std::source_location::current().function_name();
    }

    // https://stackoverflow.com/a/2112111
    static constexpr std::size_t __hash(const char *i)
    {
        return *i ?
            static_cast<std::size_t>(*i) + 33 * __hash(i + 1) :
            5381;
    }
public:
    static constexpr auto value = __hash(GetFuncName<T>().data());
};
}

#endif // ETHYL_TRAITS_UNIQUEID_H