#ifndef POTION_CORE_EVENTS_H
#define POTION_CORE_EVENTS_H

#include <Ethyl/Traits/Function.h>
#include <Ethyl/Types/TypeIndexer.h>
#include <Ethyl/Pointers.h>
#include <unordered_map>
#include <functional>
#include <stdio.h>
#include <string>
#include <string.h>
#include <type_traits>

namespace Potion::Core
{
// Should EventManager return a token to identity registered callbacks?
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
            // ETHYL_ASSERT(instance, "Instance is a nullptr!");
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

    template<typename Event, typename Fn, bool MemberFunc = false, typename Class = typename Ethyl::Traits::Function<Fn>::Class>
    static inline constexpr auto CanBeCallback = std::is_same_v<Event, Ethyl::Traits::Sanitize<Event>> && std::is_member_function_pointer_v<Fn> == MemberFunc && 
    []() {
        if constexpr (!MemberFunc)
            return std::is_invocable_v<Fn, Event&> || std::is_invocable_v<Fn>;
        else
            return std::is_invocable_v<Fn, Class&, Event&> || std::is_invocable_v<Fn, Class&>;
    }();

public:
    EventBus() = default;

    EventBus(EventBus&&) = default;
    EventBus(const EventBus&) = default;

    EventBus& operator=(EventBus&&) = default;
    EventBus& operator=(const EventBus&) = default;

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
    inline void Post([[maybe_unused]] Args&&... args)
    {
        if constexpr(std::is_empty_v<Event>) {
            union { Event event; };
            Post(event);
        } 
        else if constexpr(requires { Event(std::forward<Args>(args)...); }) 
        {
            Event event(std::forward<Args>(args)...);
            Post(event);
        } 
        else if constexpr(requires { Event{std::forward<Args>(args)...}; }) 
        {
            Event event{std::forward<Args>(args)...};
            Post(event);
        } 
        else 
        {
            []<bool T = false>() {
                static_assert(T, "Event type not constructible from args");
            }();
        }
    }

    template<typename Event>
    inline void Post(Event &event)
    {
        auto range = m_Handlers.equal_range(Ethyl::Traits::UniqueID<Event>::value);

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
    inline void Remove(void *instance)
    {
        RemoveHandler<Event, Fn>(instance);
    }

    inline void ClearAll()
    {
        m_Handlers.clear();
    }

private:
    template<typename Event, auto Fn>
    inline void AddHandler(void *instance)
    {
        Entry entry = { instance, DelegateCallback<Event, Fn> };
        m_Handlers.emplace(Ethyl::Traits::UniqueID<Event>::value, entry);
    }

    template<typename Event, auto Fn>
    inline void RemoveHandler(Ethyl::Traits::Function<decltype(Fn)>::Class *instance)
    {
        for (auto it = m_Handlers.begin(); it != m_Handlers.end();)
        {
            const auto &entry = it->second;

            if (entry.instance == instance && entry.delegate == DelegateCallback<Event, Fn>)
            {
                it = m_Handlers.erase(it);
                break;
            }
            else
                ++it;
        }
    }

    std::unordered_multimap<size_t, Entry> m_Handlers;
};
};

#endif // POTION_CORE_EVENTS_H