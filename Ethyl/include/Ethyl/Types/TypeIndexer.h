#ifndef ETHYL_TYPES_TYPEINDEXER_H
#define ETHYL_TYPES_TYPEINDEXER_H

#include <cstddef>
#include <vector>
#include <unordered_map>
#include "Ethyl/Traits/UniqueID.h"

namespace Ethyl::Types
{
class TypeIndexer final
{
public:
    template<typename T>
    inline std::size_t Get()
    {
        auto hash = Traits::UniqueID<T>::value;

        if (const auto it = mTypeIndices.find(hash); it != mTypeIndices.end())
        {
            return it->second;
        }
        else [[unlikely]]
        {
            mTypeHashes.push_back(hash);
            return mTypeIndices.emplace(hash, mTypeHashes.size() - 1).first->second;
        }
    }

    template<typename T>
    void Remove()
    {
        auto hash = Traits::UniqueID<T>::value;
        const auto it = mTypeIndices.find(hash);
        if (it == mTypeIndices.end()) return;

        auto typeIndex = it->second;

        if (typeIndex < mTypeHashes.size() - 1)
        {
            auto lastHash = mTypeHashes.back();
            mTypeHashes[typeIndex] = lastHash;
            mTypeIndices[lastHash] = typeIndex;
        }

        mTypeHashes.pop_back();
        mTypeIndices.erase(it);
    }
private:
    std::vector<std::size_t> mTypeHashes;
    std::unordered_map<std::size_t, std::size_t> mTypeIndices;
};
}

#endif // ETHYL_TYPES_TYPEINDEXER_H