#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <array>
#include <SFML/OpenGL.hpp>
#include "../component.h"
#include "transform.h"

class Model : public Component
{
public:
    std::vector<std::array<float, 3>> verticies;

    Model();
    //void cube(float size, Transform pos);
};

#endif // MODEL_H
