#include "Potion/Core/Engine.h"
#include <Ethyl/Assert.h>
#include <Tonic/Core/Time.h>

namespace Potion::Core
{
    std::unique_ptr<Engine::Context> Engine::m_Context = nullptr;

    Engine::Context &Engine::CurrentContext()
    {
        ETHYL_ASSERT(m_Context, "Engine is not initialized!");
        return *m_Context;
    }

    void Engine::Initialize()
    {
        ETHYL_ASSERT(!m_Context, "Engine is already initialized!");
        m_Context = Ethyl::CreateUnique<Context>();
    }

    template<typename ...Args>
    struct SignalHelper {};

    void Engine::Ignite(uint64_t numTicks)
    {
        auto &ctx = CurrentContext();
        ctx.m_TickRate = 1.0f / numTicks;

        const auto SignalEvents = []<typename... Events>(SignalHelper<Events...> a, auto&&... args)
        {
            (EventBus().Post<Events>(std::forward<decltype(args)>(args)...), ...);
        };

        bool isRunning = true;

        while (isRunning)
        {
            switch (ctx.m_State)
            {
            case EState::Init: [[unlikely]]
                ctx.m_TimeStart = Tonic::Core::Time::GetTickCount();
                ctx.m_TimeCurrent = ctx.m_TimeStart;
                ctx.m_TimeLast = ctx.m_TimeStart;

                SignalEvents(SignalHelper<Event::PreInit, Event::Init, Event::PostInit>{});
                if (ctx.m_State == EState::Init) ctx.m_State = EState::Running;
                break;
            case EState::Running: [[likely]]
                ctx.m_TimeLast = ctx.m_TimeCurrent;
                ctx.m_TimeCurrent = Tonic::Core::Time::GetTickCount();
                ctx.m_TimeDelta = ctx.m_TimeCurrent - ctx.m_TimeLast;

                ctx.m_TickRemainer += ctx.m_TimeDelta;

                SignalEvents(SignalHelper<Event::PreUpdate, Event::Update, Event::PostUpdate>{}, ctx.m_TimeDelta);

                for (; ctx.m_TickRemainer > ctx.m_TickRate; ctx.m_TickRemainer -= ctx.m_TickRate)
                    SignalEvents(SignalHelper<Event::PreTick, Event::Tick, Event::PostTick>{}, ctx.m_TickRate);

                SignalEvents(SignalHelper<Event::PreRender, Event::Render, Event::PostRender>{}, ctx.m_TimeDelta);
                break;
            case EState::Shutdown: [[unlikely]]
                SignalEvents(SignalHelper<Event::PreShutdown, Event::Shutdown, Event::PostShutdown>{});
                EventBus().ClearAll();
                m_Context.release();
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

    double Engine::GetRunTime()
    {
        return Tonic::Core::Time::GetTickCount() - CurrentContext().m_TimeStart;
    }

    double Engine::GetStartTime()
    {
        return CurrentContext().m_TimeStart;
    }
}