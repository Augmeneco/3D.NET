#ifndef PROGRAM_H
#define PROGRAM_H
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "camera.h"
#include "world.h"
#include "client.h"
#include "console.h"

class Program
{
public:
    static Program* instance;
    float fps;
    int maxFPS;
    bool mouseLock;
    bool consoleOpened;
    sf::RenderWindow* window;
    tgui::Gui gui;
    World* world;
    Client* client;
    Camera camera;
    Console console;


    Program();
    static Program* getInstance();
    void run();
    void render(sf::Clock& beetwenIterTime);
    GLuint loadShaders(std::string vertexFilePath, std::string fragmentFilePath);
    void renderSceneObjects(Object* obj, sf::Vector2i scenePos);
    void addSceneToLoad(sf::Vector2i sceneCoord);
};

#endif // PROGRAM_H
