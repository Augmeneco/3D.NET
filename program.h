#ifndef PROGRAM_H
#define PROGRAM_H
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "camera.h"
#include "world.h"
#include "console.h"

class Program
{
public:
    static Program* instance;
    int maxFPS = 0;
    bool mouseLock;
    bool consoleOpened;
    sf::RenderWindow* window;
    tgui::Gui gui;
    World* world;
    Camera camera;
    Console console;


    Program();
    static Program* getInstance();
    void run();
    void render();
    void renderSceneObjects(Object* obj, sf::Vector2i scenePos);
};

#endif // PROGRAM_H
