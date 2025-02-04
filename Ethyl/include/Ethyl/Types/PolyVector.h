#ifndef ETHYL_TYPES_POLYVECTOR_H
#define ETHYL_TYPES_POLYVECTOR_H

#include "Ethyl/Assert.h"
#include "Ethyl/Traits/Constructible.h"
#include "Ethyl/Traits/Name.h"

namespace Ethyl::Types
{
template<typename Base, bool EnableSameType = false>
class PolyVector final
{
private:
    using PointerType = Unique<Base, void(*)(const Base *)>;
public:
    PolyVector() = default;

    PolyVector(const PolyVector &) = default;
    PolyVector& operator=(const PolyVector&) = default;

    PolyVector(PolyVector &&) = default;
    PolyVector& operator=(PolyVector&&) = default;

    template<typename T, typename... Args>
    requires(std::derived_from<T, Base> && std::same_as<T, Base> == EnableSameType && Traits::Constructible<T, Args...>)
    inline T& Create(Args&&... args)
    {
        const auto index = mIndexer.Get<T>();
        while (index >= mValues.size()) mValues.emplace_back(nullptr, nullptr);
        ETHYL_ASSERT(!m_Values[index], "An instance of {} already exists!", Ethyl::Traits::Name<T>::value);

        Base *instance;
        if constexpr (std::is_aggregate_v<T>)
        {
            if constexpr (std::same_as<T, Base>)
            {
                instance = new T{std::forward<Args>(args)...};
            }
            else
            {
                if constexpr (!std::is_empty_v<Base>) {} // Very unlikely to ever happen
                instance = new T{{}, std::forward<Args>(args)...};
            }
        }
        else
        {
            instance = new T(std::forward<Args>(args)...);
        }

        mValues[index] = PointerType(instance, +[](const Base *base) { delete static_cast<const T *>(base); });
        return *static_cast<T *>(mValues[index].get());
    }

    template<typename... T>
    requires((std::derived_from<T, Base> && std::same_as<T, Base> == EnableSameType) && ...)
    [[nodiscard]] inline decltype(auto) Get()
    {
        if constexpr (Traits::Arguments<T...>::Arity == 1)
        {
            const auto index = mIndexer.Get<T...>();
            ETHYL_ASSERT(index < mValues.size(), "An instance of {} does not exist!", Ethyl::Traits::Name<T...>::value);
            return (*static_cast<T *>(mValues[index].get()), ...);
        }
        else
        {
            return std::forward_as_tuple(Get<T>()...);
        }
    }

    template<typename... T>
    requires((std::derived_from<T, Base> && std::same_as<T, Base> == EnableSameType) && ...)
    inline void Remove()
    {
        if constexpr (Traits::Arguments<T...>::Arity == 1)
        {
            const auto index = mIndexer.Get<T...>();
            if (mValues.size() > index) mValues[index].reset();
        }
        else
        {
            (Remove<T>(), ...);
        }
    }

    template<typename... T>
    requires((std::derived_from<T, Base> && std::same_as<T, Base> == EnableSameType) && ...)
    [[nodiscard]] inline bool Has()
    {
        if constexpr (Traits::Arguments<T...>::Arity == 1)
        {
            const auto index = mIndexer.Get<T...>();
            return mValues.size() > index && mValues[index];
        }
        else
        {
            return (Has<T>() && ...);
        }
    }

    template<typename... T>
    requires((std::derived_from<T, Base> && std::same_as<T, Base> == EnableSameType) && ...)
    [[nodiscard]] inline bool HasAny()
    {
        return (Has<T>() || ...);
    }

private:
    std::vector<PointerType> mValues;
    TypeIndexer mIndexer;
};
}

#endif // ETHYL_TYPES_POLYVECTOR_H