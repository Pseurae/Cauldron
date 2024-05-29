#ifndef POTION_CORE_EVENTS_H
#define POTION_CORE_EVENTS_H

#include <Ethyl/Traits/Function.h>
#include <Ethyl/Types/TypeIndexer.h>
#include <Ethyl/Pointers.h>
#include <unordered_map>
#include <concepts>
#include <functional>
#include <stdio.h>
#include <unordered_map>

namespace Potion::Core
{
// Should EventManager return a token to identity registered callbacks?
class EventBus final 
{
private:
    struct Entry
    {
        enum class Type { Function, Functor };

        Type type;
        void *instance;
        size_t id;

        std::function<void(const void *)> delegate;
    };

    template<typename Event, typename Fn, bool MemberFunc = false>
    static inline constexpr auto CanBeCallback = []()
    {
        if constexpr (!MemberFunc)
            return std::is_invocable_v<Fn, const Event &> || std::is_invocable_v<Fn>;
        else
            return std::is_invocable_v<Fn, typename Ethyl::Traits::Function<Fn>::Class&, const Event &> || std::is_invocable_v<Fn, typename Ethyl::Traits::Function<Fn>::Class&>;
    }() && std::is_same_v<Event, std::remove_pointer_t<std::remove_cvref_t<Event>>> && (std::is_member_function_pointer_v<Fn> == MemberFunc);

    template<typename Fn>
    static inline constexpr std::pair<size_t, Entry::Type> GetEntryIDData(void *instance, Fn callback)
    {
        if constexpr (Ethyl::Traits::Function<Fn>::IsFunctor)
            return { typeid(Fn).hash_code(), Entry::Type::Functor };
        else if constexpr (std::is_member_function_pointer_v<Fn>)
            return { typeid(Fn).hash_code(), Entry::Type::Function };
        else
            return { (size_t)callback, Entry::Type::Function };
    }

public:
    EventBus() = default;

    EventBus(EventBus&&) = default;
    EventBus(const EventBus&) = default;

    EventBus& operator=(EventBus&&) = default;
    EventBus& operator=(const EventBus&) = default;

    template<typename Event, typename Fn>
    requires(CanBeCallback<Event, Fn>)
    inline void Register(Fn callback)
    {
        AddHandler<Event>(nullptr, callback);
    }

    template<typename Event, typename Fn>
    requires(CanBeCallback<Event, Fn, true>)
    inline void Register(void *instance, Fn callback)
    {
        AddHandler<Event>(instance, callback);
    }

    template<typename Event, typename... Args>
    inline void Post([[maybe_unused]] Args&&... args)
    {
        if constexpr(std::is_empty_v<Event>) {
            union { Event event; };
            Post(event);
        } else if constexpr(requires {Event(std::forward<Args>(args)...); }) {
            Event event(std::forward<Args>(args)...);
            Post(event);
        } else if constexpr(requires {Event{std::forward<Args>(args)...}; }) {
            Event event{std::forward<Args>(args)...};
            Post(event);
        } else {
            [] <bool T = false>() {
                static_assert(T, "Event type not constructible from args");
            }();
        }
    }

    template<typename Event>
    inline void Post(const Event &event)
    {
        auto range = m_Handlers.equal_range(m_Indexer.Get<Event>());

        for (auto it = range.first; it != range.second; ++it)
            it->second.delegate(static_cast<const void *>(&event));
    }

    template<typename Event, typename Fn>
    requires(CanBeCallback<Event, Fn>)
    inline void Remove(Fn callback)
    {
        RemoveHandler<Event>(nullptr, callback);
    }

    template<typename Event, typename Fn>
    requires(CanBeCallback<Event, Fn, true>)
    inline void Remove(void *instance, Fn callback)
    {
        RemoveHandler<Event>(instance, callback);
    }

    inline void ClearAll()
    {
        m_Handlers.clear();
    }

private:
    template<typename Event, typename Fn>
    inline void AddHandler(void *instance, Fn callback)
    {
        auto [id, type] = GetEntryIDData(instance, callback);
        auto delegate = [classInstance = static_cast<typename Ethyl::Traits::Function<Fn>::Class *>(instance), func = std::forward<Fn>(callback)](const void *event) {
            if constexpr (std::is_member_function_pointer_v<Fn> && !std::is_same_v<typename Ethyl::Traits::Function<Fn>::Class, void>)
            {
                if constexpr (Ethyl::Traits::Function<Fn>::Arity == 0) (classInstance->*func)();
                else (classInstance->*func)(*static_cast<const Event *>(event));
            }
            else
            {
                if constexpr (Ethyl::Traits::Function<Fn>::Arity == 0) func();
                else func(*static_cast<const Event *>(event));
            }
        };

        Entry entry = { type, instance, id, delegate };
        m_Handlers.emplace(m_Indexer.Get<Event>(), entry);
    }

    template<typename Event, typename Fn>
    inline void RemoveHandler(void *instance, Fn callback)
    {
        auto [id, type] = GetEntryIDData(instance, callback);

        for (auto it = m_Handlers.begin(); it != m_Handlers.end();)
        {
            const auto &entry = it->second;

            if (entry.id == id && entry.type == type && entry.instance == instance)
            {
                it = m_Handlers.erase(it);
                break;
            }
            else
                ++it;
        }
    }

    Ethyl::Types::TypeIndexer m_Indexer;
    std::unordered_multimap<size_t, Entry> m_Handlers;
};
};

#endif // POTION_CORE_EVENTS_H