#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "../component.h"

class Transform : public Component
{
public:
    double x = 0;
    double y = 0;
    double z = 0;

    float scaleX = 1;
    float scaleY = 1;
    float scaleZ = 1;

    float pitch = 0;
    float yaw = 0;
    float roll = 0;

    Transform();
    Transform(float x, float y, float z);
};

#endif // TRANSFORM_H
