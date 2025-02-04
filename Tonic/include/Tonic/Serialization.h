#pragma once

#include <vector>
#include <string>
#include <type_traits>
#include <iostream>

namespace Tonic::Serialization
{
namespace impl
{
template<typename T>
struct Encoder
{};
}

class Getter final
{
public:
    Getter(const std::vector<unsigned char> &data) : mData(data) {}

    template<typename ...T>
    void operator()(T&&... args)
    {
        (impl::Encoder<std::remove_cvref_t<T>>::Deserialize(args, *this), ...);   
    }

    template<typename U>
    friend class impl::Encoder;

private:
    std::vector<unsigned char> mData;
    size_t mIndex = 0;
};

class Setter final
{
public:
    template<typename ...T>
    void operator()(T&&... args)
    {
        (impl::Encoder<std::remove_cvref_t<T>>::Serialize(args, *this), ...);   
    }

    const auto &GetData() const { return mData; }

    template<typename U>
    friend class impl::Encoder;

private:
    std::vector<unsigned char> mData;
};

namespace impl
{
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

template<typename T>
concept Arithmetic = std::is_arithmetic<std::remove_cvref_t<T>>::value;

// All Arithmetic Types
template<Arithmetic T>
struct Encoder<T>
{
    static void Serialize(T t, Setter &s)
    {
        const unsigned char *rawData = (unsigned char *)(intptr_t)&t;
        for (int i = 0; i < sizeof(T); ++i)
            s.mData.push_back(rawData[i]);
    }

    static void Deserialize(T &t, Getter &g)
    {
        unsigned char *rawData = (unsigned char *)(intptr_t)&t;
        for (int i = 0; i < sizeof(T); ++i, ++g.mIndex)
            rawData[i] = g.mData.size() > g.mIndex ? g.mData[g.mIndex] : 0;
    }
};

// Vectors
template<typename T>
struct Encoder<std::vector<T>>
{
    static void Serialize(const std::vector<T> &t, Setter &s)
    {
        Encoder<decltype(t.size())>::Serialize(t.size(), s);
        for (auto &i : t)
            Encoder<decltype(i)>::Serialize(i, s);
    }

    static void Deserialize(std::vector<T> &t, Getter &g)
    {
        size_t vectorSize;
        Encoder<size_t>::Deserialize(vectorSize, g);
        t.resize(vectorSize);

        for (int i = 0; i < vectorSize; ++i)
        {
            T elem;
            Encoder<T>::Deserialize(elem, g);
            t.push_back(elem);
        }
    }
};

// Strings
template<>
struct Encoder<std::string>
{
    static void Serialize(const std::string &t, Setter &s)
    {
        Encoder<decltype(t.size())>::Serialize(t.size(), s);
        for (auto &i : t)
            Encoder<decltype(i)>::Serialize(i, s);
    }

    static void Deserialize(std::string &t, Getter &g)
    {
        size_t stringSize;
        Encoder<size_t>::Deserialize(stringSize, g);
        t.resize(stringSize);

        char *data = t.data();

        for (int i = 0; i < stringSize; ++i)
        {
            char x;
            Encoder<char>::Deserialize(x, g);
            data[i] = x;
        }
    }
};

template<typename T>
concept ClassType = std::is_class<std::remove_cvref_t<T>>::value;

// Structures and Classes
template<ClassType T>
struct Encoder<T>
{
    static void Serialize(T &t, Setter &s)
    {
        if constexpr (HasSerializeFunction<T>)
            t.Serialize(s);
    }

    static void Deserialize(T &t, Getter &g)
    {
        if constexpr (HasDeserializeFunction<T>)
            t.Deserialize(g);
    }
};
}
}