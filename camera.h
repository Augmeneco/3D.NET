#ifndef CAMERA_H
#define CAMERA_H
#include "components/transform.h"
#include <SFML/Graphics.hpp>

class Camera
{
public:
    float sensitivity;
    bool jump = false;
    Transform transform;
    Transform renderingCoords;
    sf::Vector2i activeChunk;

    Camera();
    Camera(float x, float y, float z);
    void moveTo(float x, float y, float z);
    void move(float x, float y, float z);
    void rotateTo(float pitch, float yaw);
    void rotate(float pitch, float yaw);
};

#endif // CAMERA_H
