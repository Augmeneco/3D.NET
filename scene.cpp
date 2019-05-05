#include "scene.h"

Scene::Scene()
{
    Object* obj = new Object();
    this->rootObject = obj;
    obj->rootObject = obj;
    obj->setTag("root");

    this->lastUpdateTime = sf::Clock();
}

Object* Scene::createObject(Object* parentObj)
{
    Object* obj = new Object();
    obj->setParent(parentObj);
    obj->rootObject = this->rootObject;
    return obj;
}

Object* Scene::createObject()
{
    Object* obj = new Object();
    obj->setParent(this->rootObject);
    return obj;
}

Object* Scene::getObjectByTag(std::string tag)
{
    Object* obj = this->findRecursively(tag, this->rootObject);
    return obj;
}

Object* Scene::findRecursively(std::string tag, Object* obj)
{
    for (Object* childObj : obj->getChilds())
    {
        if (tag == childObj->getTag())
        {
            return childObj;
        }
        else
        {
            if (childObj->getChilds().size() != 0)
            {
                this->findRecursively(tag, childObj);
            }
        }
    }
}
