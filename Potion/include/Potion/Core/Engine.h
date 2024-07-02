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

struct PreUpdate { double timeDelta; };
struct Update : PreUpdate {};
struct PostUpdate : PreUpdate {};

struct PreTick { double tickRate; };
struct Tick : PreTick {};
struct PostTick : PreTick {};

struct PreRender { double timeDelta; };
struct Render : PreRender {};
struct PostRender : PreRender {};

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

        double m_TimeStart = 0.0;

        double m_TimeDelta = 0.0;

        double m_TimeLast = 0.0;
        double m_TimeCurrent = 0.0;

        double m_TickRate = 1.0 / 60.0;
        double m_TickRemainer = 0.0;
    };

    // No need for this to be a unique_ptr
    static Ethyl::Unique<Context> m_Context;
    static Context &CurrentContext();

public:
    inline static EventBus &EventBus() { return CurrentContext().m_EventBus; }

    static void Initialize();
    static void Ignite(uint64_t numTicks = 60);
    static void Shutdown();

    static double GetStartTime();
    static double GetRunTime();

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