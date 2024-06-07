#ifndef POTION_ECS_WORLD_H
#define POTION_ECS_WORLD_H

#include "Potion/ECS/Constants.h"
#include "Potion/ECS/ComponentPool.h"
#include "Potion/ECS/System.h"
#include <Ethyl/Pointers.h>
#include <Ethyl/Traits/Function.h>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <functional>
#include <iostream>
#include <ranges>
#include <set>

namespace Potion::ECS
{
class World final
{
private:
    using ComponentPoolPtr = Ethyl::Unique<_::BaseComponentPool, void(*)(const _::BaseComponentPool *)>;
    using SystemPtr = Ethyl::Unique<_::BaseSystem, void(*)(const _::BaseSystem *)>;

    struct EntityGenerator
    {
        EntityID nextID = 1;

        inline auto getNextID() { return nextID++; }
        inline auto getMax() const { return nextID; }
    };

    template<typename Component>
    inline auto GetComponentIndex()
    {
        auto idx = m_ComponentIndexer.Get<Component>();
        ETHYL_ASSERT(idx < Constants::MaxComponentTypes, "Maximum limit for components exceeded!");
        return idx;
    }

    friend class BaseSystem;

    template<typename Component>
    inline _::ComponentPool<Component>& GetComponentPool()
    {
        auto idx = GetComponentIndex<Component>();

        if (m_ComponentPools.size() <= idx) [[unlikely]]
        {
            while (idx + 1 > m_ComponentPools.size())
                m_ComponentPools.emplace_back(nullptr, nullptr);

            auto pool = ComponentPoolPtr(new _::ComponentPool<Component>(), +[](const _::BaseComponentPool *pool) {
                delete static_cast<const _::ComponentPool<Component> *>(pool);
            });

            m_ComponentPools[idx] = std::move(pool);
        }

        return *static_cast<_::ComponentPool<Component> *>(m_ComponentPools[idx].get());
    }

    template<typename T, typename... Args>
    static inline constexpr bool is_unique = !(std::is_same_v<T, Args> || ...) && is_unique<Args...>;

    template<typename T>
    static inline constexpr bool is_unique<T> = true; 

    template<typename System, typename Components, std::size_t... I, typename... Args>
    inline System& RegisterSystemImpl(std::index_sequence<I...>, Args&&... args)
    {
        std::bitset<Constants::MaxComponentTypes> requiredMask = ((1 << GetComponentIndex<typename Components::SanitizedTypes::template NthType<I>>()) | ...);

        auto idx = m_SystemIndexer.Get<System>();
        if (m_Systems.size() <= idx) [[unlikely]]
        {
            while (idx + 1 > m_Systems.size()) m_Systems.emplace_back(nullptr, nullptr);
        }

        _::BaseSystem *instance;
        if constexpr (std::is_aggregate_v<System>)
            instance = new System{std::forward<Args>(args)...};
        else
            instance = new System(std::forward<Args>(args)...);
        
        m_Systems[idx] = SystemPtr(instance, +[](const _::BaseSystem *system) { delete static_cast<const System *>(system); });

        instance->m_Mask = requiredMask;
        for (const auto &[id, mask] : m_EntityToMask | std::views::filter([&requiredMask](auto it) { return (it.second & requiredMask) == requiredMask; }))
        {
            instance->m_AffectedEntities.emplace(id);
            m_EntityToSystems[id].emplace(idx);
        }

        m_MaskToSystem.emplace(requiredMask, idx);
        return *static_cast<System *>(m_Systems[idx].get());
    }

    template<typename System, std::size_t... I>
    inline void RunSystemImpl(System &system, std::index_sequence<I...>)
    {
        ForEach<typename System::RequiredComponents::SanitizedTypes::template NthType<I>...>([&system](EntityID id, typename System::RequiredComponents::Types::template NthType<I>... args) {
            system.Invoke(id, args...);
        });
    }

    inline void RemoveEntityFromOldSystems(EntityID id)
    {
        auto oldMask = m_EntityToMask[id];
        auto range = m_MaskToSystem.equal_range(oldMask);
        for (auto it = range.first; it != range.second; ++it)
        {
            m_Systems[it->second]->m_AffectedEntities.erase(id);
            m_EntityToSystems[id].erase(it->second);
        }
    }

    inline void AddEntityToNewSystems(EntityID id)
    {
        auto newMask = m_EntityToMask[id];
        auto range = m_MaskToSystem.equal_range(newMask);
        for (auto it = range.first; it != range.second; ++it)
        {
            m_Systems[it->second]->m_AffectedEntities.emplace(id);
            m_EntityToSystems[id].emplace(it->second);
        }
    }

public:
    static constexpr EntityID InvalidEntity = 0;

    inline EntityID CreateEntity() 
    {
        ComponentPoolPtr t = ComponentPoolPtr(nullptr, nullptr);
        auto entityID = m_Generator.getNextID();
        m_EntityToMask[entityID] = {};
        m_EntityToSystems[entityID] = {};
        return entityID; 
    }
    
    inline void DestroyEntity(EntityID id) 
    {
        auto mask = m_EntityToMask[id];
        for (std::size_t i = 0; i < Constants::MaxComponentTypes; ++i)
        {
            if (mask[i]) m_ComponentPools[i]->Remove(id);
        }

        for (auto &systemIdx: m_EntityToSystems[id]) m_Systems[systemIdx]->m_AffectedEntities.erase(id);

        m_EntityToMask.erase(id);
        m_EntityToSystems.erase(id);
    }

    template<typename Component>
    Component& AssignComponent(EntityID id, Component&& component)
    {
        _::ComponentPool<Component>& pool = GetComponentPool<Component>();

        RemoveEntityFromOldSystems(id);
        m_EntityToMask[id].set(GetComponentIndex<Component>());
        AddEntityToNewSystems(id);

        return pool.Assign(id, std::move(component));
    }

    template<typename Component, typename... Args>
    Component& AssignComponent(EntityID id, Args&&... args)
    {
        if constexpr (std::is_aggregate_v<Component>)
            return AssignComponent(id, Component{std::forward<Args>(args)...});
        else
            return AssignComponent(id, Component(std::forward<Args>(args)...));
    }

    template<typename Component>
    [[nodiscard]] Component& RetrieveComponent(EntityID id)
    {
        ETHYL_ASSERT(m_EntityToMask[id][GetComponentIndex<Component>()], "An instance of {} is not assigned to entity of id {}!", Ethyl::Traits::Name<Component>::value, id);
        return GetComponentPool<Component>().Get(id);
    }

    template<typename Component>
    void RemoveComponent(EntityID id)
    {
        ETHYL_ASSERT(m_EntityToMask.find(id) != m_EntityToMask.end(), "Entity of id {} does not exist!", id);
        RemoveEntityFromOldSystems(id);
        m_EntityToMask[id].reset(GetComponentIndex<Component>());
        AddEntityToNewSystems(id);

        GetComponentPool<Component>().Remove(id);
    }

    template<typename... Components, typename Func, 
        typename FuncTraits = Ethyl::Traits::Function<Func>, typename Arguments = Ethyl::Traits::Arguments<EntityID, Components...>>
    requires(/* is_unique<Components...> && */ FuncTraits::Arity == (1 + sizeof...(Components)) && 
        std::is_same_v<typename Arguments::Types, typename Arguments::SanitizedTypes>)
    void ForEach(Func func)
    {
        std::bitset<Constants::MaxComponentTypes> requiredMask = ((1 << GetComponentIndex<Components>()) | ...);
        for (const auto &[id, entityMask] : m_EntityToMask)
        {
            if ((requiredMask & entityMask) == requiredMask) func(id, RetrieveComponent<Components>(id)...);
        }
    }

    template<typename... Components, typename Func, 
        typename FuncTraits = Ethyl::Traits::Function<Func>, typename Arguments = Ethyl::Traits::Arguments<EntityID, Components...>>
    requires(/* is_unique<Components...> && */ FuncTraits::Arity == (1 + sizeof...(Components)) && 
        std::is_same_v<typename Arguments::Types, typename Arguments::SanitizedTypes>)
    std::function<void()> System(Func func)
    {
        return [f = std::forward<Func>(func), this](){ this->ForEach<Components...>(f); };
    }

    template<typename System, typename... Args>
    [[nodiscard]] System& RegisterSystem(Args&&... args) 
    {
        ETHYL_ASSERT(m_Systems.size() <= m_SystemIndexer.Get<System>() || !m_Systems[m_SystemIndexer.Get<System>()], "{} is already registered!", Ethyl::Traits::Name<System>::value);
        return RegisterSystemImpl<System, typename System::RequiredComponents>(std::make_index_sequence<System::RequiredComponents::Arity>{}, std::forward<Args>(args)...);
    }

    template<typename System>
    void RunSystem(System &system)
    {
        RunSystemImpl(system, std::make_index_sequence<System::RequiredComponents::Arity>{});
    }

    template<typename T, typename System = std::remove_pointer_t<std::remove_cvref_t<T>>>
    void RemoveSystem()
    {
        ETHYL_ASSERT(m_Systems.size() > m_SystemIndexer.Get<System>() && m_Systems[m_SystemIndexer.Get<System>()], "{} isn't registered yet!", Ethyl::Traits::Name<System>::value);
        m_Systems[m_SystemIndexer.Get<System>()] = nullptr;
    }

private:
    EntityGenerator m_Generator;
    Ethyl::Types::TypeIndexer m_ComponentIndexer, m_SystemIndexer;
    std::unordered_map<EntityID, std::bitset<Constants::MaxComponentTypes>> m_EntityToMask;
    std::vector<ComponentPoolPtr> m_ComponentPools;

    std::vector<SystemPtr> m_Systems;
    std::unordered_map<EntityID, std::set<std::size_t>> m_EntityToSystems;
    std::unordered_multimap<std::bitset<Constants::MaxComponentTypes>, std::size_t> m_MaskToSystem;
};
}

#endif // POTION_ECS_WORLD_H