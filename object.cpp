#include "exceptions.h"
#include "components/transform.h"
#include "components/model.h"
#include "object.h"
#include <typeinfo>
#include <iostream>


Object::Object()
{
    this->model = nullptr;
    this->transform = nullptr;
    this->childs = std::vector<Object*>();
}

Object::~Object()
{
}

bool Object::setTag(std::string tag)
{
    Object* obj = findRecursively(tag, this->rootObject);
    if (obj)
    {
        this->tag = tag;
        return true;
    }
    else
    {
        return false;
    }
}

std::string Object::getTag()
{
    return this->tag;
}

bool Object::setParent(Object* parentObj)
{
    this->parent = parentObj;
    parentObj->addChild(this);
    return true;
}

Object* Object::getParent()
{
    return this->parent;
}

void Object::addChild(Object* child)
{
    this->childs.push_back(child);
}

void Object::removeChild(Object* child)
{
    //this->childs.r
}

Object* Object::getChildByTag(std::string tag)
{
    return findRecursively(tag, this);
}

std::vector<Object*> Object::getChilds()
{
    return this->childs;
}

// protected

Object* Object::findRecursively(std::string tag, Object* obj)
{
    for (Object* childObj : obj->getChilds())
    {
        if (tag == childObj->getTag())
        {
            return childObj;
        }
        else
        {
            if (childObj->childs.size() != 0)
            {
                this->findRecursively(tag, childObj);
            }
        }
    }
}
