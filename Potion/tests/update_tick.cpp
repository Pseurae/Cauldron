#include <iostream>
#include <thread>
#include <Potion/Core/Engine.h>
#include <chrono>

std::chrono::time_point<std::chrono::system_clock> tp1, tp2;

void tick(Potion::Core::Event::Tick &t)
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = now - tp1;
    std::cout << "Tick: " << t.tickRate << " " << duration << std::endl;
    tp1 = now;
}

void update(Potion::Core::Event::Update &t)
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = now - tp2;
    std::cout << "Update: " << t.timeDelta << " " << duration << std::endl;
    tp2 = now;
}

int main(int, char *[]) 
{
    using namespace Potion::Core;
    using namespace std::chrono_literals;

    try 
    {
        tp1 = std::chrono::system_clock::now();
        tp2 = tp1;

        auto close = [](){ static int counter = 0; if (counter++ > 5) Engine::Shutdown(); };

        Engine::Initialize();
        Engine::EventBus().Register<Event::Tick, &tick>();
        Engine::EventBus().Register<Event::Update, &update>();
        Engine::EventBus().Register<Event::PostTick, close>();
        Engine::Ignite(30);
        return 0;
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 1;
}