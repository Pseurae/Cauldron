#ifndef ETHYL_TYPES_TYPEINDEXER_H
#define ETHYL_TYPES_TYPEINDEXER_H

#include <cstddef>
#include <vector>
#include <unordered_map>
#include "Ethyl/Traits/UniqueID.h"

namespace Ethyl::Types
{
class TypeIndexer
{
public:
    template<typename T>
    inline std::size_t Get()
    {
        auto hash = Traits::UniqueID<T>::value;

        if (const auto it = m_TypeIndices.find(hash); it != m_TypeIndices.end())
        {
            return it->second;
        }
        else [[unlikely]]
        {
            m_TypeHashes.push_back(hash);
            return m_TypeIndices.emplace(hash, m_TypeHashes.size() - 1).first->second;
        }
    }

    template<typename T>
    void Remove()
    {
        auto hash = Traits::UniqueID<T>::value;
        const auto it = m_TypeIndices.find(hash);
        if (it == m_TypeIndices.end()) return;

        auto typeIndex = it->second;

        if (typeIndex < m_TypeHashes.size() - 1)
        {
            auto lastHash = m_TypeHashes.back();
            m_TypeHashes[typeIndex] = lastHash;
            m_TypeIndices[lastHash] = typeIndex;
        }

        m_TypeHashes.pop_back();
        m_TypeIndices.erase(it);
    }
private:
    std::vector<std::size_t> m_TypeHashes;
    std::unordered_map<std::size_t, std::size_t> m_TypeIndices;
};
}

#endif // ETHYL_TYPES_TYPEINDEXER_H