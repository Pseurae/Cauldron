#ifndef POTION_UTILITIES_EVENTBUS_H
#define POTION_UTILITIES_EVENTBUS_H

#include <Ethyl/Traits/Function.h>
#include <Ethyl/Types/TypeIndexer.h>
#include <Ethyl/Pointers.h>
#include <Ethyl/Traits/Constructible.h>
#include <Ethyl/Assert.h>
#include <map>
#include <functional>
#include <type_traits>

namespace Tonic
{
class EventBus final 
{
private:
    template<typename Event, auto Fn>
    static void DelegateCallback(void *instance, void *event)
    {
        using FuncTraits = Ethyl::Traits::Function<decltype(Fn)>;
        constexpr auto TakeNoArgs = FuncTraits::Arity == 0;

        if constexpr (FuncTraits::IsMemberFunction)
        {
            ETHYL_ASSERT(instance, "Instance is a nullptr!");
            if constexpr (TakeNoArgs) (static_cast<typename FuncTraits::Class *>(instance)->*Fn)();
            else (static_cast<typename FuncTraits::Class *>(instance)->*Fn)(*static_cast<Event *>(event));
        }
        else
        {
            if constexpr (TakeNoArgs) Fn();
            else Fn(*static_cast<Event *>(event));
        }
    }

    struct Entry
    {
        void *instance;
        void (*delegate)(void *instance, void *event);
    };

    template<typename Event, typename Fn, bool MemberFunc = false, typename Class = typename Ethyl::Traits::Function<Fn>::Class, bool IsFunctor = Ethyl::Traits::Function<Fn>::IsFunctor>
    static inline constexpr auto CanBeCallback = std::is_same_v<Event, Ethyl::Traits::Sanitize<Event>> && std::is_member_function_pointer_v<Fn> == MemberFunc && 
    []() {
        if constexpr (!MemberFunc)
            return (std::is_invocable_v<Fn, Event&> || std::is_invocable_v<Fn>) && !IsFunctor;
        else
            return std::is_invocable_v<Fn, Class&, Event&> || std::is_invocable_v<Fn, Class&>;
    }();

public:
    EventBus() = default;

    EventBus(EventBus&&) = default;
    EventBus(const EventBus&) = default;

    EventBus& operator=(EventBus&&) = default;
    EventBus& operator=(const EventBus&) = default;

/*
    template<auto Fn, typename FuncTraits = Ethyl::Traits::Function<decltype(Fn)>, typename = std::enable_if_t<FuncTraits::Arity == 1>, typename Event = typename FuncTraits::SanitizedTypes::template NthType<0>>
    requires(CanBeCallback<Event, decltype(Fn)>)
    inline void Register()
    {
        AddHandler<Event, Fn>(nullptr);
    }

    template<auto Fn, typename FuncTraits = Ethyl::Traits::Function<decltype(Fn)>, typename = std::enable_if_t<FuncTraits::Arity == 1>, typename Event = typename FuncTraits::SanitizedTypes::template NthType<0>>
    requires(CanBeCallback<Event, decltype(Fn), true>)
    inline void Register(FuncTraits::Class *instance)
    {
        AddHandler<Event, Fn>(instance);
    }
*/

    template<typename Event, auto Fn>
    requires(CanBeCallback<Event, decltype(Fn)>)
    inline void Register()
    {
        AddHandler<Event, Fn>(nullptr);
    }

    template<typename Event, auto Fn>
    requires(CanBeCallback<Event, decltype(Fn), true>)
    inline void Register(Ethyl::Traits::Function<decltype(Fn)>::Class *instance)
    {
        AddHandler<Event, Fn>(instance);
    }

    template<typename Event, typename... Args>
    requires(Ethyl::Traits::Constructible<Event, Args...>)
    inline void Post([[maybe_unused]] Args&&... args)
    {
        if constexpr (std::is_aggregate_v<Event>) Post(Event{std::forward<Args>(args)...});
        else Post(Event(std::forward<Args>(args)...));
    }

    template<typename Event>
    inline void Post(Event &&event)
    {
        auto range = mHandlers.equal_range(Ethyl::Traits::UniqueID<Event>::value);

        for (auto it = range.first; it != range.second; ++it)
            it->second.delegate(it->second.instance, static_cast<void *>(&event));
    }

    template<typename Event, auto Fn>
    requires(CanBeCallback<Event, decltype(Fn)>)
    inline void Remove()
    {
        RemoveHandler<Event, Fn>(nullptr);
    }

    template<typename Event, auto Fn>
    requires(CanBeCallback<Event, decltype(Fn), true>)
    inline void Remove(Ethyl::Traits::Function<decltype(Fn)>::Class *instance)
    {
        RemoveHandler<Event, Fn>(instance);
    }

    template<typename... Events>
    inline void RemoveEvents()
    {
        if constexpr (Ethyl::Traits::Arguments<Events...>::Arity == 1)
            mHandlers.erase(Ethyl::Traits::UniqueID<Events...>::value);
        else
            (RemoveEvent<Events>(), ...);
    }

    inline void ClearAll()
    {
        mHandlers.clear();
    }

private:
    template<typename Event, auto Fn>
    inline void AddHandler(void *instance)
    {
        Entry entry = { instance, DelegateCallback<Event, Fn> };
        mHandlers.emplace(Ethyl::Traits::UniqueID<Event>::value, entry);
    }

    template<typename Event, auto Fn>
    inline void RemoveHandler(void *instance)
    {
        for (auto it = mHandlers.begin(); it != mHandlers.end();)
        {
            const auto &entry = it->second;

            if (entry.instance == instance && entry.delegate == DelegateCallback<Event, Fn>)
            {
                it = mHandlers.erase(it);
                break;
            }
            else
                ++it;
        }
    }

    std::multimap<size_t, Entry> mHandlers;
};
};

#endif // POTION_UTILITIES_EVENTBUS_H