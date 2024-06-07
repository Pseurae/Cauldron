#ifndef POTION_ECS_SYSTEM_H
#define POTION_ECS_SYSTEM_H

#include "Potion/ECS/Constants.h"
#include <bitset>
#include <set>

namespace Potion::ECS
{
class World;

namespace _
{
class BaseSystem
{
protected:
    std::bitset<Constants::MaxComponentTypes> m_Mask;
    std::set<EntityID> m_AffectedEntities;

    friend class ECS::World;
};
}

template<typename... Arguments>
class System : public _::BaseSystem
{
public:
    using RequiredComponents = Ethyl::Traits::Arguments<Arguments...>;
    virtual void Invoke(EntityID, Arguments...) {};
};
}

#endif // POTION_ECS_SYSTEM_H