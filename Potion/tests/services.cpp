#include <Potion/Core/Engine.h>
#include <iostream>

using namespace Potion::Core;

class TestService
{
public:
    TestService(int a) : m_Test(a)
    {
        Engine::EventBus().Register<Event::PostInit, +[](){ std::cout << "PostInit hook from TestService!" << std::endl; Engine::Shutdown(); }>();
    }

    inline int GetInt() const { return m_Test; }
private:
    int m_Test;
};



int main(int argc, char *argv[])
{
    Engine::Initialize();
    std::cout << "Engine::HasService<TestService>(): " << Engine::HasService<TestService>() << std::endl;
    Engine::EventBus().Register<Event::RegisteredService<TestService>, +[](){ std::cout << "TestService registered!" << std::endl; }>();
    Engine::RegisterService<TestService>(10);
    std::cout << "Engine::HasService<TestService>(): " << Engine::HasService<TestService>() << std::endl;

    auto service = Engine::GetService<TestService>();
    std::cout << "TestService::m_Test is " << service.GetInt() << std::endl;
    Engine::Ignite();
    return 0;
}