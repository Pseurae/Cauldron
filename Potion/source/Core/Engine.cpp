#include "Potion/Core/Engine.h"
#include <Ethyl/Assert.h>

namespace Potion::Core
{
    std::unique_ptr<Engine::Context> Engine::m_Context = nullptr;

    Engine::Context &Engine::CurrentContext()
    {
        ETHYL_ASSERT(m_Context, "Engine is not initialized!");
        return *m_Context;
    }

    Services &Engine::Services()
    {
        return CurrentContext().m_Services;
    }

    EventBus &Engine::EventBus()
    {
        return CurrentContext().m_EventBus;
    }

    void Engine::Initialize()
    {
        ETHYL_ASSERT(!m_Context, "Engine is already initialized!");
        m_Context = Ethyl::CreateUnique<Context>();
    }

    template<typename ...Args>
    struct SignalHelper {};

    void Engine::Ignite()
    {
        auto &ctx = CurrentContext();

        const auto SignalEvents = []<typename... Events>(SignalHelper<Events...> a, auto&&... args)
        {
            (EventBus().Post<Events>(args...), ...);
        };

        bool isRunning = true;
        while (isRunning)
        {
            switch (ctx.m_State)
            {
            case EState::Init: [[unlikely]]
                SignalEvents(SignalHelper<Event::PreInit, Event::Init, Event::PostInit>{});
                ctx.m_State = EState::Running;
                break;
            case EState::Running: [[likely]]
                SignalEvents(SignalHelper<Event::PreUpdate, Event::Update, Event::PostUpdate>{});
                // SignalEvents(SignalHelper<Event::PreTick, Event::Tick, Event::PostTick>{});
                SignalEvents(SignalHelper<Event::PreRender, Event::Render, Event::PostRender>{});
                break;
            case EState::Shutdown: [[unlikely]]
                SignalEvents(SignalHelper<Event::PreShutdown, Event::Shutdown, Event::PostShutdown>{});
                EventBus().ClearAll();
                isRunning = false;
                break;
            }
        }
    }

    void Engine::Shutdown()
    {
        ETHYL_ASSERT(m_Context, "Engine is not initialized!");
        m_Context->m_State = EState::Shutdown;
    }
}