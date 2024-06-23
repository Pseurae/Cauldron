#ifndef POTION_CORE_ENGINE_H
#define POTION_CORE_ENGINE_H

#include <Ethyl/Pointers.h>

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
    };

    // No need for this to be a unique_ptr
    static Ethyl::Unique<Context> m_Context;
    static Context &CurrentContext();

public:
    static EventBus &EventBus() { return CurrentContext().m_EventBus; }

    static void Initialize();
    static void Ignite();
    static void Shutdown();
};
}

#endif // POTION_CORE_ENGINE_H