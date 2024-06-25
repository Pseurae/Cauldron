#ifndef ETHYL_TYPES_TYPEVECTOR_H
#define ETHYL_TYPES_TYPEVECTOR_H

#include "Ethyl/Assert.h"
#include "Ethyl/Traits/Arguments.h"
#include "Ethyl/Traits/Constructible.h"
#include "Ethyl/Types/TypeIndexer.h"
#include "Ethyl/Traits/Name.h"
#include <any>

namespace Ethyl::Types
{
class TypeVector final
{
public:
    TypeVector() = default;

    TypeVector(const TypeVector &) = default;
    TypeVector& operator=(const TypeVector&) = default;

    TypeVector(TypeVector &&) = default;
    TypeVector& operator=(TypeVector&&) = default;

    template<typename T, typename... Args>
    requires(Traits::Constructible<T, Args...>)
    inline T& Create(Args&&... args)
    {
        const auto index = m_Indexer.Get<T>();
        if (index >= m_Values.size()) m_Values.resize(index + 1);
        ETHYL_ASSERT(!m_Values[index].has_value(), "An instance of {} already exists!", Ethyl::Traits::Name<T>::value);

        if constexpr (std::is_aggregate_v<T>) m_Values[index] = T{std::forward<Args>(args)...};
        else m_Values[index] = T(std::forward<Args>(args)...);

        return std::any_cast<T&>(m_Values[index]);
    }

    template<typename... T>
    [[nodiscard]] inline decltype(auto) Get()
    {
        if constexpr (Traits::Arguments<T...>::Arity == 1)
        {
            const auto index = m_Indexer.Get<T...>();
            ETHYL_ASSERT(index < m_Values.size(), "An instance of {} does not exist!", Ethyl::Traits::Name<T...>::value);
            return (std::any_cast<T&>(m_Values[index]), ...);
        }
        else
        {
            return std::forward_as_tuple(Get<T>()...);
        }
    }

    template<typename... T>
    inline void Remove()
    {
        if constexpr (Traits::Arguments<T...>::Arity == 1)
        {
            const auto index = m_Indexer.Get<T...>();
            if (m_Values.size() > index) m_Values[index].reset();
        }
        else
        {
            (Remove<T>(), ...);
        }
    }

    template<typename... T>
    [[nodiscard]] inline bool Has()
    {
        if constexpr (Traits::Arguments<T...>::Arity == 1)
        {
            const auto index = m_Indexer.Get<T...>();
            return m_Values.size() > index && m_Values[index].has_value();
        }
        else
        {
            return (Has<T>() && ...);
        }
    }

    template<typename... T>
    [[nodiscard]] inline bool HasAny()
    {
        return (Has<T>() || ...);
    }

private:
    std::vector<std::any> m_Values;
    TypeIndexer m_Indexer;
};
}

#endif // ETHYL_TYPES_TYPEVECTOR_H