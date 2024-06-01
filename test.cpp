#include <Potion/Core/Engine.h>

struct Test 
{
    int i;
};

#include <iostream>

using namespace Potion::Core;
#include <Ethyl/Types/TypeVector.h>

int main(int, char *[]) 
{ 
    try 
    {
        Ethyl::Types::TypeVector typeVector;
        // typeVector.Create<Test>(2);
        // typeVector.Create<Test>(1);

        auto &t = typeVector.Get<Test>();
        std::cout << t.i << std::endl;
        std::cout << typeVector.Has<Test>() << std::endl;
        typeVector.Remove<Test>();
        std::cout << typeVector.Has<Test>() << std::endl;
        return 0;
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 1;
}