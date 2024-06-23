#ifndef POTION_ECS_COMPONENTPOOL_H
#define POTION_ECS_COMPONENTPOOL_H

#include "Potion/ECS/Constants.h"
#include <Ethyl/Assert.h>

namespace Potion::ECS::_
{
struct BaseComponentPool 
{
    virtual void Remove(EntityID) {};
};

template<typename Component>
class ComponentPool : public BaseComponentPool
{
public:
    Component& Assign(EntityID id, Component&& component)
    {
        ETHYL_ASSERT(m_Components.find(id) == m_Components.end(), "Component already exists!");
        m_Components[id] = std::move(component);
        return m_Components[id];
    }

    Component& Get(EntityID id)
    {
        ETHYL_ASSERT(m_Components.find(id) != m_Components.end(), "Component doesn't exist!");
        return m_Components[id];
    }

    virtual void Remove(EntityID id) override
    {
        m_Components.erase(id);
    }

private:
    std::unordered_map<EntityID, Component> m_Components;
};
}

#endif // POTION_ECS_COMPONENTPOOL_H