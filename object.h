#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include <vector>
#include <map>
#include <array>
#include "components/transform.h"
#include "components/model.h"


class Object
{
protected:
    std::string tag;
    std::vector<Object*> childs;
    Object* parent;

    Object* findRecursively(std::string tag, Object* obj);

public:
    Object* rootObject;

    Transform* transform;
    Model* model;
    std::map<std::string, Component*> components;

    Object();
    ~Object();

    bool setTag(std::string tag);
    std::string getTag();

    bool setParent(Object* obj);
    Object* getParent();

    void addChild(Object* child);
    void removeChild(Object* child);
    Object* getChildByTag(std::string);
    std::vector<Object*> getChilds();
};

#endif // OBJECT_H
