#include <Potion/Core/Engine.h>

struct Test { int i; int j; };

using namespace Potion::Core;

int main(int, char *[]) 
{ 
    Engine::Initialize();
    Engine::EventBus().Register<Event::Update>([]() { printf("Test\n"); });
    Engine::Ignite();
    return 0;
}