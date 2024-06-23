#include <Potion/Core/Engine.h>

using namespace Potion;

struct test1 { int i; };

void test_func(test1 &)
{
    printf("Test");
}

int main(int argc, char *argv[])
{
    Core::Engine::Initialize();
    Core::Engine::EventBus().Register<test_func>();
    Core::Engine::EventBus().Post<test1>();
    Core::Engine::EventBus().Remove<test1, test_func>();
    Core::Engine::EventBus().Post<test1>();
    return 0;
}