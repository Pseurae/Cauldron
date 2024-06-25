#include <Potion/Core/Engine.h>

using namespace Potion;

struct test1 { int i; };

void test_func(test1 &t)
{
    printf("test_func called!\n");
    printf("test1::i is %i\n", t.i);
}

class TestClass
{
public:
    void TestEmpty()
    {
        printf("TestEmpty called!\nTestClass::m_F is %f\n", m_F);
    }

    void TestCustom(test1 &t)
    {
        printf("TestCustom called!\nTestClass::m_I is %i\n", m_I);
        printf("test1::i is %i\n", t.i);
    }
private:
    int m_I = 69;
    float m_F = 3.14f;
};

int main(int argc, char *argv[])
{
    Core::Engine::Initialize();
    Core::Engine::EventBus().Register<&test_func>();

    TestClass t;
    Core::Engine::EventBus().Register<&TestClass::TestCustom>(&t);

    Core::Engine::EventBus().Post<test1>();
    Core::Engine::EventBus().Post<test1>(10);
    return 0;
}