#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "object.h"
#include "components/transform.h"

class Scene
{
public:
    std::mutex* sceneLock;
    sf::Vector2i coordinates;
    sf::IpAddress ip;
    sf::Clock lastUpdateTime;
    Object* rootObject;

    Scene();
    Object* createObject();
    Object* createObject(Object* parentObj);
    Object* getObjectByTag(std::string tag);

private:
    Object* findRecursively(std::string tag, Object* obj);
};

#endif // SCENE_H
