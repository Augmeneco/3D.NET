#include <stdio.h>
#include "camera.h"

Camera::Camera()
{
    this->sensitivity = 0.05f;

    this->transform.x = 0;
    this->transform.y = 1;
    this->transform.z = 0;
}

Camera::Camera(float x, float y, float z)
{
    this->sensitivity = 0.05f;

    this->transform.x = x;
    this->transform.y = y;
    this->transform.z = z;

    this->activeChunk = sf::Vector2i(0, 0);
}

void Camera::moveTo(float x, float y, float z)
{
    this->transform.x = x;
    this->transform.y = y;
    this->transform.z = z;
}

void Camera::move(float x, float y, float z)
{

    this->transform.x += x;
    this->transform.y += y;
    this->transform.z += z;
}

void Camera::rotateTo(float pitch, float yaw)
{
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    if(yaw > 359.0f)
        yaw = yaw - 360.0f;
    if(yaw < 0.0f)
        yaw = 360.0f + yaw;

    this->transform.pitch = pitch;
    this->transform.yaw = yaw;
}

void Camera::rotate(float pitch, float yaw)
{
    this->transform.pitch += pitch * -this->sensitivity;
    this->transform.yaw += yaw * -this->sensitivity;

    if(this->transform.pitch > 89.0f)
        this->transform.pitch = 89.0f;
    if(this->transform.pitch < -89.0f)
        this->transform.pitch = -89.0f;

    if(this->transform.yaw > 359.0f)
        this->transform.yaw = this->transform.yaw - 360.0f;
    if(this->transform.yaw < 0.0f)
        this->transform.yaw = 360.0f + this->transform.yaw;
}
