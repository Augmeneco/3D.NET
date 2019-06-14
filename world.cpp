#include "world.h"

World* World::instance = nullptr;

World::World()
{
    this->scenesArrayMutex = new std::mutex();
}

World* World::getInstance()
{
    if(!World::instance)
        World::instance = new World();
    return World::instance;
}
