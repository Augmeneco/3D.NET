#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <array>
#include <string>
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "../component.h"
#include "transform.h"

class Model : public Component
{
public:
    GLuint vertexBufObjID;
    GLuint normalBufObjID;

    std::string model_uri;
    std::vector<std::array<float, 3>> verticies;
    std::vector<std::array<float, 2>> uvs;
    std::vector<std::array<float, 3>> normals;
    bool textureLoaded;
    sf::Image textureImage;
    sf::Texture texture;

    Model();
    //void cube(float size, Transform pos);
    void loadOBJ(std::string& objText);
};

#endif // MODEL_H
