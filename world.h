#ifndef WORLD_H
#define WORLD_H
#include <map>
#include <queue>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "components/transform.h"
#include "scene.h"

class World
{
public:
    sf::Thread loadNearScenesThread;
    std::vector<Scene*> scenes;
    std::queue<sf::Vector2i> queueToLoad;
    sf::Mutex* scenesArrayMutex;
    sf::Mutex* queueToLoadMutex;

    World();

    void parseResult(auto sceneUnparsed, Object* parentObj, Scene* scene);
    sf::Vector2i ipToDim(sf::IpAddress ip, int w);
    sf::IpAddress dimToIp(sf::Vector2i vec, int w);
    std::vector<std::string> explode(std::string const & s, char delim);
};

#endif // WORLD_H
