#ifndef ETHYL_TYPES_TYPEVECTOR_H
#define ETHYL_TYPES_TYPEVECTOR_H

#include "Ethyl/Assert.h"
#include "Ethyl/Types/TypeIndexer.h"
#include "Ethyl/Traits/Name.h"
#include <any>

namespace Ethyl::Types
{
class TypeVector
{
public:
    TypeVector() = default;

    TypeVector(const TypeVector &) = default;
    TypeVector& operator=(const TypeVector&) = default;

    TypeVector(TypeVector &&) = default;
    TypeVector& operator=(TypeVector&&) = default;

    template<typename T, typename... Args>
    inline T& Create(Args&&... args)
    {
        const auto index = m_Indexer.Get<T>();
        if (index >= m_Values.size()) m_Values.resize(index + 1);
        ETHYL_ASSERT(!m_Values[index].has_value(), "An instance of {} already exists!", Ethyl::Traits::Name<T>::value);

        if constexpr (std::is_aggregate_v<T>)
            m_Values[index] = T{std::forward<Args>(args)...};
        else
            m_Values[index] = T(std::forward<Args>(args)...);

        return std::any_cast<T&>(m_Values[index]);
    }

    template<typename T>
    [[nodiscard]] inline T &Get()
    {
        const auto index = m_Indexer.Get<T>();
        ETHYL_ASSERT(index < m_Values.size(), "An instance of {} does not exist!", Ethyl::Traits::Name<T>::value);
        return std::any_cast<T&>(m_Values[index]);
    }

    template<typename T>
    inline void Remove()
    {
        const auto index = m_Indexer.Get<T>();
        if (m_Values.size() > index) m_Values[index].reset();
    }

    template<typename T>
    [[nodiscard]] inline bool Has()
    {
        const auto index = m_Indexer.Get<T>();
        return m_Values.size() > index && m_Values[index].has_value();
    }

private:
    std::vector<std::any> m_Values;
    TypeIndexer m_Indexer;
};
}

#endif // ETHYL_TYPES_TYPEVECTOR_H