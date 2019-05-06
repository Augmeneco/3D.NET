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
    this->window = new sf::RenderWindow(sf::VideoMode(600, 600), "3D.NET", sf::Style::Default, sf::ContextSettings(32, 0, 8));
    this->gui.setTarget(*this->window);

    this->console.init();

    this->window->setVerticalSyncEnabled(true);
    this->window->setActive(true);
    this->window->setMouseCursorVisible(false);

    // load resources, initialize the OpenGL states, ...
    //prepare OpenGL surface for HSR
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport(0, 0, this->window->getSize().x, this->window->getSize().x);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    this->addSceneToLoad(this->camera.activeChunk);
    for (int x = this->camera.activeChunk.x - 3; x < this->camera.activeChunk.x + 3; x++)
        for (int y = this->camera.activeChunk.y - 3; y < this->camera.activeChunk.y + 3; y++)
            if ((x >= 0) && (x <= 65565) &&
                (y >= 0) && (y <= 65565))
                if (this->camera.activeChunk != sf::Vector2i(x, y))
                    this->addSceneToLoad(sf::Vector2i(x, y));

    // run the main loop
    bool running = true;
    sf::Event event;
    sf::Clock beetwenIterTime = sf::Clock();
    sf::Clock fpsUpdateTimer = sf::Clock();
    sf::Clock cleanGarScenesTimer = sf::Clock();
    while (running)
    {
        if (fpsUpdateTimer.getElapsedTime().asMilliseconds() >= 500)
        {
            this->fps = 1.f / beetwenIterTime.getElapsedTime().asSeconds();
            fpsUpdateTimer.restart();
        }
        beetwenIterTime.restart();

        if (cleanGarScenesTimer.getElapsedTime().asSeconds() >= 180)
        {
            this->world->scenesArrayMutex->lock();
            Scene* scene = nullptr;
            for (int i = 0; i < this->world->scenes.size(); i++)
                if (this->world->scenes[i]->lastUpdateTime.getElapsedTime().asSeconds() >= 180)
                {
                    delete this->world->scenes[i];
                    this->world->scenes.erase(this->world->scenes.begin() + i);
                    break;
                }
            this->world->scenesArrayMutex->unlock();
        }

        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::GainedFocus)
            {
                this->mouseLock = true;
                this->window->setMouseCursorVisible(false);
            }
            if (event.type == sf::Event::LostFocus)
            {
                this->mouseLock = false;
                this->window->setMouseCursorVisible(true);
            }
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            if (event.type == sf::Event::Resized)
            {
                this->window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                this->gui.setView(window->getView());
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
            sleep(this->maxFPS);
    }
}

void Program::render()
{
    sf::Vector2i localPosition = sf::Mouse::getPosition(*this->window);
    //printf("%d %d\n", (this->window->getSize().x / 2), (this->window->getSize().y / 2));
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity() ;
    gluPerspective(45.0f, (float)this->window->getSize().x / (float)this->window->getSize().y, 0.1f, 100.0f);
    //gluPerspective(45.0f, (float)600 / (float)600, 0.1f, 100.0f);

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


    this->camera.renderingCoords.x = this->camera.transform.x - (this->camera.activeChunk.x * 20) - 10;
    this->camera.renderingCoords.y = 1;
    this->camera.renderingCoords.z = this->camera.transform.z - (this->camera.activeChunk.y * 20) - 10;


    if ((this->camera.activeChunk.x <= 0) && (this->camera.renderingCoords.x <= -10))
    {
        this->camera.renderingCoords.x = -10;
        this->camera.transform.x = 0;
    }
    if ((this->camera.activeChunk.x >= 65536) && (this->camera.renderingCoords.x >= 10))
    {
        this->camera.renderingCoords.x = 10;
        this->camera.transform.x = 65536 * 20;
    }

    if ((this->camera.activeChunk.y <= 0) && (this->camera.renderingCoords.z <= -10))
    {
        this->camera.renderingCoords.z = -10;
        this->camera.transform.z = 0;
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
    else if (this->camera.renderingCoords.x < -10)
    {
        this->camera.activeChunk.x -= 1;
    }
    if (this->camera.renderingCoords.z > 10)
    {
        this->camera.activeChunk.y += 1;
    }
    else if (this->camera.renderingCoords.z < -10)
    {
        this->camera.activeChunk.y -= 1;
    }


    if ((this->camera.renderingCoords.x < -10) || (this->camera.renderingCoords.x > 10) ||
        (this->camera.renderingCoords.z < -10) || (this->camera.renderingCoords.z > 10))
    {
        printf("Current chunk IP: %s\n", this->world->dimToIp(this->camera.activeChunk, 65565).toString().c_str());

        this->camera.renderingCoords.x = (this->camera.transform.x) - (this->camera.activeChunk.x * 20) - 10;
        this->camera.renderingCoords.y = 1;
        this->camera.renderingCoords.z = (this->camera.transform.z) - (this->camera.activeChunk.y * 20) - 10;

        this->addSceneToLoad(this->camera.activeChunk);
        for (int x = this->camera.activeChunk.x - 3; x < this->camera.activeChunk.x + 3; x++)
            for (int y = this->camera.activeChunk.y - 3; y < this->camera.activeChunk.y + 3; y++)
                if ((x >= 0) && (x <= 65565) &&
                    (y >= 0) && (y <= 65565))
                    if (this->camera.activeChunk != sf::Vector2i(x, y))
                        this->addSceneToLoad(sf::Vector2i(x, y));
    }


    gluLookAt(this->camera.renderingCoords.x,
              this->camera.renderingCoords.y,
              this->camera.renderingCoords.z,
              this->camera.renderingCoords.x+rotX,
              this->camera.renderingCoords.y+rotY,
              this->camera.renderingCoords.z-rotZ,
              0,1,0);

    this->window->setTitle("FPS: " + std::to_string(this->fps) + " | " +
                           std::to_string(this->camera.renderingCoords.x) + " " +
                           std::to_string(this->camera.renderingCoords.z) + " | " +
                           std::to_string(this->camera.transform.x) + " " +
                           std::to_string(this->camera.transform.z) + " | " +
                           std::to_string(this->camera.activeChunk.x) + " "+
                           std::to_string(this->camera.activeChunk.y));


    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glPushMatrix();
        glLoadIdentity();
        glTranslatef(1.0, 1.0, 1.0);
        GLfloat light0_position[] = { 0.0, 0.0, 0.0, 1.0 };
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glPopMatrix();

    this->world->scenesArrayMutex->lock();
    for (Scene* s : this->world->scenes)
        if (s->coordinates == this->camera.activeChunk)
        {
            glPushMatrix();
            glTranslatef(1.0f, 0.0f, 1.0f);
            this->renderSceneObjects(s->rootObject, s->coordinates);
            glPopMatrix();
            break;
        }

    this->world->scenesArrayMutex->unlock();
    this->world->scenesArrayMutex->lock();

    for (int x = this->camera.activeChunk.x - 3; x < this->camera.activeChunk.x + 3; x++)
        for (int y = this->camera.activeChunk.y - 3; y < this->camera.activeChunk.y + 3; y++)
            for (Scene* s : this->world->scenes)
                if ((s->coordinates == sf::Vector2i(x, y)) && (s->coordinates != this->camera.activeChunk))
                {
                    glPushMatrix();
                    glTranslatef((this->camera.activeChunk.x - x) * -20.0f + 1.0f, 0.0f, (this->camera.activeChunk.y - y) * -20.0f + 1.0f);
                    this->renderSceneObjects(s->rootObject, s->coordinates);
                    glPopMatrix();
                    break;
                }
    this->world->scenesArrayMutex->unlock();

    for (int x = this->camera.activeChunk.x - 3; x < this->camera.activeChunk.x + 3; x++)
        for (int y = this->camera.activeChunk.y - 3; y < this->camera.activeChunk.y + 3; y++)
            if ((x >= 0) && (x <= 65565) &&
                (y >= 0) && (y <= 65565))
            {
                glPushMatrix();
                glTranslatef((this->camera.activeChunk.x - x) * -20.0f, 0.0f, (this->camera.activeChunk.y - y) * -20.0f);
                glBegin(GL_LINES);
                    glColor3f(0.7f, 0.5f, 0.7f);
                    glVertex3f(-10.0f, 0.0f, -10.0f);
                    glVertex3f(10.0f, 0.0f, -10.0f);

                    glVertex3f(10.0f, 0.0f, -10.0f);
                    glVertex3f(10.0f, 0.0f, 10.0f);

                    glVertex3f(10.0f, 0.0f, 10.0f);
                    glVertex3f(-10.0f, 0.0f, 10.0f);

                    glVertex3f(-10.0f, 0.0f, 10.0f);
                    glVertex3f(-10.0f, 0.0f, -10.0f);
                glEnd();
                glTranslatef(1.0f, 0.0f, 1.0f);
                glBegin(GL_LINES);
                    glColor3f(1.0f, 0.0f, 0.0f);
                    glVertex3f(-9.0f, 0.0f, 0.0f);
                    glVertex3f(9.0f, 0.0f, 0.0f);

                    glColor3f(0.0, 1.0f, 0.0f);
                    glVertex3f(0.0f, -1.0f, 0.0f);
                    glVertex3f(0.0f, 1.0f, 0.0f);

                    glColor3f(0.0f, 0.0f, 1.0f);
                    glVertex3f(0.0f, 0.0f, -9.0f);
                    glVertex3f(0.0f, 0.0f, 9.0f);
                glEnd();
                glBegin(GL_LINES);
                    glColor3f(0.7f, 0.5f, 0.7f);
                    glVertex3f(-9.0f, 0.0f, -9.0f);
                    glVertex3f(9.0f, 0.0f, -9.0f);

                    glVertex3f(9.0f, 0.0f, -9.0f);
                    glVertex3f(9.0f, 0.0f, 9.0f);

                    glVertex3f(9.0f, 0.0f, 9.0f);
                    glVertex3f(-9.0f, 0.0f, 9.0f);

                    glVertex3f(-9.0f, 0.0f, 9.0f);
                    glVertex3f(-9.0f, 0.0f, -9.0f);
                glEnd();
                glPopMatrix();
            }
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 1.0f);
    glPointSize(10);
    glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(9.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 9.0f);
    glEnd();
    glPopMatrix();

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

void Program::addSceneToLoad(sf::Vector2i sceneCoord)
{
    this->world->scenesArrayMutex->lock();
    Scene* scene = nullptr;
    for (Scene* s : this->world->scenes)
        if (s->coordinates == sceneCoord)
        {
            scene = s;
            break;
        }
    this->world->scenesArrayMutex->unlock();

    bool loadThisScene = false;
    if (scene != nullptr)
    {
        if (scene->lastUpdateTime.getElapsedTime().asSeconds() >= 60)
        {
            loadThisScene = true;
        }
    }
    else
    {
        scene = new Scene();
        scene->ip = this->world->dimToIp(sceneCoord, 65565);
        scene->coordinates = sceneCoord;
        this->world->scenesArrayMutex->lock();
        this->world->scenes.push_back(scene);
        this->world->scenesArrayMutex->unlock();
        loadThisScene = true;
    }

    if (loadThisScene == true)
    {
        this->world->queueToLoadMutex->lock();
        this->world->queueToLoad.push(sceneCoord);
        this->world->queueToLoadMutex->unlock();
    }
}

void Program::renderSceneObjects(Object* obj, sf::Vector2i scenePos)
{
    glPushMatrix();
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
                    glNormal3f(obj->model->verticies[i][0], obj->model->verticies[i][1], obj->model->verticies[i][2]);
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
