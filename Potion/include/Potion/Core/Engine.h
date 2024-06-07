#ifndef POTION_CORE_ENGINE_H
#define POTION_CORE_ENGINE_H

#include <Tonic/Input/Keyboard.h>
#include <Tonic/Input/Mouse.h>

#include <Tonic/Graphics/Window.h>
#include <Tonic/Graphics/Device.h>
#include <Ethyl/Pointers.h>

#include "Potion/Core/Events.h"
#include "Potion/Core/Services.h"

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
        Services m_Services;
        EventBus m_EventBus;
    };

    // No need for this to be a unique_ptr
    static std::unique_ptr<Context> m_Context;
    static Context &CurrentContext();

public:
    static Services &Services();
    static EventBus &EventBus();

    static void Initialize();
    static void Ignite();
    static void Shutdown();
};
}

#endif // POTION_CORE_ENGINE_H