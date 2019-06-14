#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <queue>
#include <string>
#include <mutex>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "components/transform.h"
#include "scene.h"

class World
{
public:
    static World* instance;
    static World* getInstance();

    std::vector<Scene*> scenes;
    std::mutex* scenesArrayMutex;

    World();
};

#endif // WORLD_H
