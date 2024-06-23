#ifndef ETHYL_TRAITS_NAME_H
#define ETHYL_TRAITS_NAME_H

#include <string_view>
#include <source_location>
#include <array>

namespace Ethyl::Traits
{
template<typename T>
struct Name
{
private:
    template <std::size_t...Idxs>
    [[nodiscard]] static inline constexpr auto substring_as_array(std::string_view str, std::index_sequence<Idxs...>)
    {
        return std::array{str[Idxs]..., '\0'};
    }

    template<typename>
    [[nodiscard]] static inline consteval auto getFunctionSignature()
    {
        return std::string_view{std::source_location::current().function_name()};
    }

    // template<typename>
    [[nodiscard]] static inline consteval auto getTypeName() {
        constexpr auto signature = getFunctionSignature<T>();
    #if defined(__clang__)
        constexpr auto prefix = std::string_view{"T = "};
        constexpr auto suffix = std::string_view{"]"};
    #elif defined(__GNUC__)
        constexpr auto prefix = std::string_view{"T = "};
        constexpr auto suffix = std::string_view{"]"};
    #elif defined(_MSC_VER)
        constexpr auto prefix = std::string_view{"getFunctionSignature<"};
        constexpr auto suffix = std::string_view{">(void)"};
    #else
        #error Unsupported compiler!
    #endif

        constexpr auto start = signature.find(prefix) + prefix.size();
        constexpr auto end = signature.find(suffix, start);
        constexpr auto name = signature.substr(start, end - start);

        return substring_as_array(name, std::make_index_sequence<name.size()>{});
    }

    static inline constexpr auto valueAsArray = getTypeName();
public:
    static inline constexpr auto value = valueAsArray.data();
};
}

#endif // ETHYL_TRAITS_NAME_H