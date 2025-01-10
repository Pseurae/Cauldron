#pragma once

#include <vector>
#include <type_traits>

namespace Tonic::Serialization
{
class Getter;
class Setter;

template <typename T>
concept HasSerializeFunction = 
requires(T t) {
    { t.Serialize(std::declval<Setter &>()) };
};

template <typename T>
concept HasDeserializeFunction = 
requires(T t) {
    { t.Deserialize(std::declval<Getter &>()) };
};

class Getter final
{
public:
    Getter(const std::vector<unsigned char> &data) : mData(data) {}

    template<typename ...T>
    void operator()(T&... args)
    {
        (Deserialize<T>(args), ...);   
    }

private:
    template<typename T>
    void Deserialize(T& t)
    {
        if constexpr (std::is_arithmetic_v<T>)
        {
            unsigned char *data = (unsigned char *)(intptr_t)&t;
            
            for (int i = 0; i < sizeof(T); ++i, ++mIndex)
                data[i] = mData[mIndex];
        }
        else if constexpr (HasDeserializeFunction<T>)
        {
            t.Deserialize(*this);
        }
        else
        {
            [] <bool t = false>() {
                static_assert(t, "Type not deserializable.");
            }();
        }

    }

    std::vector<unsigned char> mData;
    size_t mIndex = 0;
};

class Setter final
{
public:
    template<typename ...T>
    void operator()(T&&... args)
    {
        (Serialize<T>(args), ...);   
    }

    const auto &GetData() const { return mData; }

private:
    template<typename T>
    void Serialize(T &t)
    {
        if constexpr (std::is_arithmetic_v<std::remove_cvref_t<T>>)
        {
            const unsigned char *data = (unsigned char *)(intptr_t)&t;
            for (int i = 0; i < sizeof(T); ++i)
                mData.push_back(data[i]);
        }
        else if constexpr (HasSerializeFunction<T>)
        {
            t.Serialize((Setter &)*this);
        }
        else
        {
            []<bool t = false>() {
                static_assert(t, "Type not serializable.");
            }();
        }
    }

    std::vector<unsigned char> mData;
};
}