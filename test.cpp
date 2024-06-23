#include <Potion/Core/Engine.h>
#include <Potion/ECS/World.h>
#include <Potion/ECS/System.h>
#include <Ethyl/Types/PolyVector.h>

#include <iostream>

using namespace Potion::Core;
using namespace Potion::ECS;

struct Position 
{
    int x, y;
};

struct Velocity
{
    int x, y;
};

struct PhysicsSystem : public System<Position, Velocity>
{
    void Invoke(EntityID id, Position& pos, Velocity& vel)
    {
        std::cout << "Physics system" << std::endl;
        std::cout << "Entity ID: " << id << std::endl;
        std::cout << "Pos: " << pos.x << " " << pos.y << std::endl;
        std::cout << "Vel: " << vel.x << " " << vel.y << std::endl;

        vel.y = 100;
    };
};

struct PositionSystem : public System<Position>
{
    void Invoke(EntityID id, Position& pos)
    {
        std::cout << "Position system" << std::endl;
        std::cout << "Entity ID: " << id << std::endl;
        std::cout << "Pos: " << pos.x << " " << pos.y << std::endl;
    };
};

int main(int, char *[]) 
{ 
    try 
    {
        Engine::Initialize();

        // std::cout << Engine::Services().Has<TestService1, TestService2, TestService3>() << std::endl;

        World world;
        auto entity1 = world.CreateEntity();

        world.AssignComponent<Position>(entity1, 10, 20);
        world.AssignComponent<Velocity>(entity1, 1, 5);

        // auto t = world.System<Position, Velocity>(+[](EntityID id, const Position &pos, Velocity &vel) {
        //     std::cout << id << std::endl;
        //     std::cout << pos.x << " " << pos.y << std::endl;
        //     std::cout << vel.x << " " << vel.y << std::endl;
        //     vel.y = 100;
        // });

        // t();

        auto &physicsSystem = world.RegisterSystem<PhysicsSystem>();
        auto &positionSystem = world.RegisterSystem<PositionSystem>();

        world.RunSystem(physicsSystem);
        world.RunSystem(positionSystem);

        std::cout << "----" << std::endl;

        auto entity2 = world.CreateEntity();

        world.AssignComponent<Position>(entity2, 15, 20);
        world.AssignComponent<Velocity>(entity2, 20, 5);

        world.RunSystem(physicsSystem);
        world.RunSystem(positionSystem);

        // world.DestroyEntity(entity1);
        world.RemoveComponent<Velocity>(entity1);

        std::cout << "----" << std::endl;

        world.RunSystem(physicsSystem);
        world.RunSystem(positionSystem);

        std::cout << "----" << std::endl;

        world.AssignComponent<Velocity>(entity1, { 10, 20 });

        world.RunSystem(physicsSystem);
        world.RunSystem(positionSystem);

        world.RemoveSystem<decltype(physicsSystem)>();
        world.RemoveSystem<decltype(positionSystem)>();

        return 0;
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 1;
}