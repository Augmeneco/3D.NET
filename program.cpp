#include <string>
#include <unistd.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "program.h"


Program* Program::instance = nullptr;


Program::Program()
{
    this->camera = Camera(0, 1, 0);
    this->world = new World();
    this->console = Console();
    this->mouseLock = true;
    this->consoleOpened = false;

    printf("%d %d\n", this->world->ipToDim(sf::IpAddress("127.0.0.1"), 65565).x, this->world->ipToDim(sf::IpAddress("127.0.0.1"), 65565).y);
    printf("%d %d\n", this->world->ipToDim(sf::IpAddress("79.165.17.222"), 65565).x, this->world->ipToDim(sf::IpAddress("79.165.17.222"), 65565).y);
//    this->world->queueToLoadMutex->lock();
//    this->world->queueToLoad.push(sf::Vector2i(40628, 32497));
//    this->world->queueToLoadMutex->unlock();
}

Program* Program::getInstance()
{
    if(!Program::instance)
        Program::instance = new Program();
    return Program::instance;
}

void Program::run()
{
    // create the window
    this->window = new sf::RenderWindow(sf::VideoMode(600, 600), "3D.NET", sf::Style::Default, sf::ContextSettings(32));
    this->gui.setTarget(*this->window);

    this->console.init();

    this->window->setVerticalSyncEnabled(true);
    this->window->setActive(true);
    this->window->setMouseCursorVisible(false);

    // load resources, initialize the OpenGL states, ...
    //prepare OpenGL surface for HSR
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Setup a perspective projection & Camera position
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    // run the main loop
    bool running = true;
    sf::Event event;
    while (running)
    {
        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::F12)
                {
                    auto consolePanel = this->gui.get<tgui::Panel>("ConsolePanel");
                    auto consoleInput = consolePanel->get<tgui::EditBox>("InputEditBox");
                    if (this->consoleOpened == false)
                    {
                        this->consoleOpened = true;
                        consolePanel->setEnabled(true);
                        consolePanel->setVisible(true);
                        consoleInput->setFocused(true);
                        //this->mouseLock = false;
                    }
                    else
                    {
                        this->consoleOpened = false;
                        consolePanel->setEnabled(false);
                        consolePanel->setVisible(false);
                        consoleInput->setFocused(false);
                        //this->mouseLock = true;
                    }
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    if (this->mouseLock == false)
                    {
                        this->mouseLock = true;
                        this->window->setMouseCursorVisible(false);
                    }
                    else
                    {
                        this->mouseLock = false;
                        this->window->setMouseCursorVisible(true);
                    }
                }
            }
            this->gui.handleEvent(event);
        }
        this->render();
        if (this->maxFPS != 0)
            sleep(maxFPS);
    }
}

void Program::render()
{
    sf::Vector2i localPosition = sf::Mouse::getPosition(*this->window);
    float deltaX = localPosition.x - 300;
    float deltaY = localPosition.y - 300;
    float sensitivity = 0.05f;
    if (this->mouseLock == true)
    {
        this->camera.rotate(deltaY, deltaX);
        sf::Mouse::setPosition(sf::Vector2i(300, 300), *this->window);
    }

    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 600, 600); //Область видимости на форме

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ; // Загружаем единичную матрицу
    gluPerspective(45.0f, (float)600 / (float)600, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float rotX = cos(glm::radians(this->camera.transform.pitch)) * cos(glm::radians(this->camera.transform.yaw));
    float rotY = sin(glm::radians(this->camera.transform.pitch));
    float rotZ = cos(glm::radians(this->camera.transform.pitch)) * sin(glm::radians(this->camera.transform.yaw));

    float moveX = cos(glm::radians(this->camera.transform.yaw));
    float moveZ = sin(glm::radians(this->camera.transform.yaw));


    #define SPEED 0.05
    if (!this->consoleOpened) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            this->camera.move(moveX*SPEED, 0, -moveZ*SPEED);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            this->camera.move(-moveX*SPEED, 0, moveZ*SPEED);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            this->camera.move(-moveZ*SPEED, 0, -moveX*SPEED);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            this->camera.move(moveZ*SPEED, 0, moveX*SPEED);
    }


    this->camera.renderingCoords.x = (this->camera.transform.x / 2) - (this->camera.activeChunk.x * 20);
    this->camera.renderingCoords.y = 1;
    this->camera.renderingCoords.z = (this->camera.transform.z / 2) - (this->camera.activeChunk.y * 20);


    if ((this->camera.activeChunk.x <= 0) && (this->camera.renderingCoords.x <= -10))
    {
        this->camera.renderingCoords.x = -10;
        this->camera.transform.x = -20;
    }
    if ((this->camera.activeChunk.x >= 65536) && (this->camera.renderingCoords.x >= 10))
    {
        this->camera.renderingCoords.x = 10;
        this->camera.transform.x = 65536 * 20;
    }

    if ((this->camera.activeChunk.y <= 0) && (this->camera.renderingCoords.z <= -10))
    {
        this->camera.renderingCoords.z = -10;
        this->camera.transform.z = -20;
    }
    if ((this->camera.activeChunk.y >= 65536) && (this->camera.renderingCoords.z >= 10))
    {
        this->camera.renderingCoords.z = 10;
        this->camera.transform.z = 65536 * 20;
    }


    if (this->camera.renderingCoords.x > 10)
    {
        this->camera.activeChunk.x += 1;
    }
    //else if ((this->camera.renderingCoords.x <= -10) && (this->camera.activeChunk.x > 0))
    else if (this->camera.renderingCoords.x < -10)
    {
        this->camera.activeChunk.x -= 1;
    }

    if (this->camera.renderingCoords.z > 10)
    {
        this->camera.activeChunk.y += 1;
    }
    //else if ((this->camera.renderingCoords.x <= -10) && (this->camera.activeChunk.x > 0))
    else if (this->camera.renderingCoords.z < -10)
    {
        this->camera.activeChunk.y -= 1;
    }


    if ((this->camera.renderingCoords.x < -10) || (this->camera.renderingCoords.x > 10) ||
        (this->camera.renderingCoords.z < -10) || (this->camera.renderingCoords.z > 10))
    {
        //прогрузка нового чанка
        this->world->loadedScenesMutex->lock();
        bool alreadyLoaded = false;
        for (std::pair<sf::Vector2i, Scene*> scene : this->world->loadedScenes)
            if (scene.first == this->camera.activeChunk)
            {
                alreadyLoaded = true;
                break;
            }
        this->world->loadedScenesMutex->unlock();

        printf("Current chunk IP: %s\n", this->world->dimToIp(this->camera.activeChunk, 65565).toString().c_str());

        if (!alreadyLoaded)
        {
            this->world->queueToLoadMutex->lock();
            this->world->queueToLoad.push(this->camera.activeChunk);
            this->world->queueToLoadMutex->unlock();
        }
    }


    gluLookAt(this->camera.renderingCoords.x,
              this->camera.renderingCoords.y,
              this->camera.renderingCoords.z,
              this->camera.renderingCoords.x+rotX,
              this->camera.renderingCoords.y+rotY,
              this->camera.renderingCoords.z-rotZ,
              0,1,0);
//    gluLookAt(0,
//              1,
//              0,
//              rotX,
//              rotY,
//              -rotZ,
//              0,1,0);

    this->window->setTitle(std::to_string(this->camera.renderingCoords.x) + " " +
                           std::to_string(this->camera.renderingCoords.z) + " | " +
                           std::to_string(this->camera.transform.x) + " " +
                           std::to_string(this->camera.transform.z) + " | " +
                           std::to_string(this->camera.activeChunk.x) + " "+
                           std::to_string(this->camera.activeChunk.y));


    this->world->loadedScenesMutex->lock();

    glPushMatrix();
    for (std::pair<sf::Vector2i, Scene*> scene : this->world->loadedScenes)
        if (scene.first == this->camera.activeChunk)
        {
            this->renderSceneObjects(scene.second->rootObject, scene.first);
            break;
        }
    glPopMatrix();

//    for (std::pair<sf::Vector2i, Scene*> scene : this->world->loadedScenes)
//    {
//        glPushMatrix();
//        //glTranslatef(scene.first.x, 0, scene.first.y);
//        this->renderSceneObjects(scene.second->rootObject, scene.first);
//        glPopMatrix();
//    }
    this->world->loadedScenesMutex->unlock();

    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-10.0f, 0.0f, 0.0f);
        glVertex3f(10.0f, 0.0f, 0.0f);
        glColor3f(0.0, 1.0f, 0.0f);
        glVertex3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -10.0f);
        glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();

    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(10.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 10.0f);
    glEnd();

    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex3f(this->camera.renderingCoords.x+rotX, this->camera.renderingCoords.y+rotY, this->camera.renderingCoords.z-rotZ);
    glEnd();

    this->window->pushGLStates();
    this->gui.draw();
    this->window->popGLStates();

    this->window->display();
}

void Program::renderSceneObjects(Object* obj, sf::Vector2i scenePos)
{
    glPushMatrix();
        //printf("%d\n", obj->model->verticies.size());
        if (obj->transform != nullptr)
        {
            glTranslatef(obj->transform->x, obj->transform->y, obj->transform->z);
            glScalef(obj->transform->scaleX, obj->transform->scaleY, obj->transform->scaleZ);
            glRotatef(obj->transform->pitch, 1, 0, 0);
            glRotatef(obj->transform->yaw, 0, 1, 0);
            glRotatef(obj->transform->roll, 0, 0, 1);
        }
        if (obj->model != nullptr)
        {
                glColor3f(1.0f, 0.0f, 1.0f);
                glBegin(GL_TRIANGLES);
                for (int i = 0; i < obj->model->verticies.size(); i++)
                {
                    glVertex3f(obj->model->verticies[i][0], obj->model->verticies[i][1], obj->model->verticies[i][2]);
                }
                glEnd();

        }

        for (Object* childObj : obj->getChilds())
        {
            this->renderSceneObjects(childObj, scenePos);
        }
    glPopMatrix();
}
