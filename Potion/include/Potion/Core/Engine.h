#ifndef POTION_CORE_ENGINE_H
#define POTION_CORE_ENGINE_H

#include <Ethyl/Pointers.h>
#include <Ethyl/Types/TypeVector.h>
#include <Ethyl/Assert.h>

#include "Potion/Core/Events.h"

namespace Potion::Core
{
namespace Event
{
struct PreInit {};
struct Init {};
struct PostInit {};

struct PreUpdate {};
struct Update {};
struct PostUpdate {};

struct PreTick {};
struct Tick {};
struct PostTick{};

struct PreRender {};
struct Render {};
struct PostRender {};

struct PreShutdown {};
struct Shutdown {};
struct PostShutdown {};

template<typename T>
struct RegisteredService {};

template<typename T>
struct UnregisteredService {};
};

class Engine final
{
private:
    enum class EState
    {
        Init,
        Running,
        Shutdown,
    };

    class Context
    {
    private:
        friend class Engine;

        EState m_State;
        EventBus m_EventBus;
        Ethyl::Types::TypeVector m_Services;
    };

    // No need for this to be a unique_ptr
    static Ethyl::Unique<Context> m_Context;
    static Context &CurrentContext();

public:
    inline static EventBus &EventBus() { return CurrentContext().m_EventBus; }

    static void Initialize();
    static void Ignite();
    static void Shutdown();

    template<typename Service, typename... Args>
    inline static void RegisterService(Args&&... args)
    {
        if (m_Context->m_State == EState::Shutdown) [[unlikely]] return;
        ETHYL_ASSERT(!CurrentContext().m_Services.Has<Service>(), "{} is already a registered service.", Ethyl::Traits::Name<Service>::value);
        EventBus().Post<Event::RegisteredService<Service>>();
        CurrentContext().m_Services.Create<Service>(std::forward<Args>(args)...);
    }

    template<typename Service>
    inline static void UnregisterService()
    {
        ETHYL_ASSERT(CurrentContext().m_Services.Has<Service>(), "{} is not a registered service.", Ethyl::Traits::Name<Service>::value);
        EventBus().Post<Event::UnregisteredService<Service>>();
        CurrentContext().m_Services.Remove<Service>();
    }

    template<typename... Services>
    inline static bool HasService()
    {
        return CurrentContext().m_Services.template Has<Services...>();
    }

    template<typename... Services>
    inline static bool HasAnyService()
    {
        return CurrentContext().m_Services.template HasAny<Services...>();
    }

    template<typename... Services>
    inline static decltype(auto) GetService()
    {
        return CurrentContext().m_Services.template Get<Services...>();
    }
};
}

#endif // POTION_CORE_ENGINE_H