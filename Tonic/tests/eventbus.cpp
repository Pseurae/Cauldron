#include <iostream>
#include <Tonic/Utilities/EventBus.h>

struct EmptyEvent {};

struct Event1 
{
    int a;
};

void TestEmptyEventWithoutArgs(void)
{
    std::cout << "TestEmptyEventWithoutArgs called." << std::endl;
}

void TestEmptyEventWithArgs(EmptyEvent &e)
{
    std::cout << "TestEmptyEventWithArgs called." << std::endl;
}

void TestEvent1WithoutArgs(void)
{
    std::cout << "TestEvent1WithoutArgs called." << std::endl;
}

void TestEvent1WithArgs(Event1 &e)
{
    std::cout << "TestEvent1WithArgs called. (" << e.a << ")" << std::endl;
}

int main(int argc, char *argv[])
{
    Tonic::EventBus eventBus;
    eventBus.Register<EmptyEvent, TestEmptyEventWithoutArgs>();
    eventBus.Register<EmptyEvent, TestEmptyEventWithArgs>();
    eventBus.Register<Event1, TestEvent1WithoutArgs>();
    eventBus.Register<Event1, TestEvent1WithArgs>();

    eventBus.Post<EmptyEvent>();
    eventBus.Post<Event1>(1);
    eventBus.Post(Event1{2});

    eventBus.RemoveEvents<EmptyEvent>();
    eventBus.RemoveEvents<Event1>();

    eventBus.Post<EmptyEvent>();
    eventBus.Post<Event1>(1);
    eventBus.Post(Event1{2});

    return 0;
}