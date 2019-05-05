#include <sstream>
#include <string.h>
#include <iostream>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "nlohmann/json.hpp"
#include "world.h"

using json = nlohmann::json;

void loadNearScenesThreadFunc(World* world)
{
    while (true)
    {
        world->queueToLoadMutex->lock();
        if (world->queueToLoad.empty())
        {
            world->queueToLoadMutex->unlock();
            continue;
        }

        sf::Vector2i coordinatesToLoad = world->queueToLoad.front();
        world->queueToLoad.pop();
        world->queueToLoadMutex->unlock();

        sf::IpAddress ipToLoad = world->dimToIp(coordinatesToLoad, 65565);

        printf("Connecting to chunk on IP: %s\n", ipToLoad.toString().c_str());

        const char req_get[] = "GET";
        sf::TcpSocket socket;
        socket.setBlocking(true);
        sf::Socket::Status status = socket.connect(ipToLoad, 53534, sf::seconds(5.0f));
        if (status != sf::Socket::Done)
        {
            printf("Socket not connected\n");
            continue;
        }

        if (socket.send(req_get, 4) != sf::Socket::Done)
        {
            printf("Socket data not sended\n");
            continue;
        }
        std::size_t receivedBytes;
        char buffer[100];
        std::string response;
        do
        {
            memset(buffer, 0, sizeof buffer);
            if(socket.receive(buffer, 100, receivedBytes) != sf::Socket::Done)
            {
                printf("Socket data not received\n");
                break;
            }
            //Теперь обрезает начало у каждого буффера
            //std::copy(&buffer[1], &buffer[receivedBytes], response.end());
            response += buffer;
            //puts(buffer);
        }
        while (receivedBytes >= 100);

//        sf::Packet packResponse;
//        socket.receive(packResponse)
//        std::string response;
//        if (!(packResponse >> response))
//        {
//            puts("Socket data not received\n");
//            continue;
//        }

        //puts(response.c_str());

        try
        {
            int lineCount = 0;
            std::string body;
            bool prevLineIsBlank = false;
            for (std::string line : world->explode(response, '\n'))
            {
                //std::cout << line << std::endl;
                if (lineCount == 0)
                {
                    if ((world->explode(line, ' ')[1] != "200"))
                    {
                        puts("Response status isn't OK\n");
                        throw 1;
                    }
                }
                if (prevLineIsBlank)
                {
                    body += line + '\n';
                }
                if (line == "")
                    prevLineIsBlank = true;

                lineCount++;
            }
            //std::cout << "BODY:\n" << body.c_str() << std::endl;
            auto sceneUnparsed = json::parse(body);

            Scene* scene = nullptr;

            world->scenesArrayMutex->lock();
            for (Scene* s : world->scenes)
                if (s->coordinates == coordinatesToLoad)
                    scene = s;
            world->scenesArrayMutex->unlock();

            for(auto childNode : sceneUnparsed["childs"])
            {
                world->parseResult(childNode, scene->rootObject, scene);
            }

            scene->lastUpdateTime.restart();

            world->scenesArrayMutex->lock();
            world->scenes.push_back(scene);
            world->scenesArrayMutex->unlock();
        }
        catch(...)
        {
            printf("Error while parse server response\n");
            world->scenesArrayMutex->unlock();
            continue;
        }
    }
}

World::World(): loadNearScenesThread(&loadNearScenesThreadFunc, this)
{
    this->scenesArrayMutex = new sf::Mutex();
    this->queueToLoadMutex = new sf::Mutex();

    this->loadNearScenesThread.launch();
}

void World::parseResult(auto node, Object* parentObj, Scene* scene)
{
    Object* obj;

    if (node["tag"] != "root")
    {
        obj = scene->createObject(parentObj);

        obj->setTag(node["tag"]);

        if (node["components"].find("transform") != node["components"].end())
        {
            obj->transform = new Transform();
            obj->transform->x = node["components"]["transform"]["x"];
            obj->transform->y = node["components"]["transform"]["y"];
            obj->transform->z = node["components"]["transform"]["z"];
            obj->transform->scaleX = node["components"]["transform"]["scaleX"];
            obj->transform->scaleY = node["components"]["transform"]["scaleY"];
            obj->transform->scaleZ = node["components"]["transform"]["scaleZ"];
            obj->transform->pitch = node["components"]["transform"]["pitch"];
            obj->transform->yaw = node["components"]["transform"]["yaw"];
            obj->transform->roll = node["components"]["transform"]["roll"];
        }
        if (node["components"].find("model") != node["components"].end())
        {
            obj->model = new Model();
            for (auto vertex : node["components"]["model"]["verticies"])
            {
                obj->model->verticies.push_back(std::array<float, 3>{{vertex[0], vertex[1], vertex[2]}});
                //printf("%g, %g, %g\n", (float)vertex[0], (float)vertex[1], (float)vertex[2]);
            }
        }
    }

    if (node["tag"] == "root")
        obj = scene->rootObject;
    for(auto childNode : node["childs"])
    {
        this->parseResult(childNode, obj, scene);
    }
}


sf::Vector2i World::ipToDim(sf::IpAddress ip, int w)
{
    int i = ip.toInteger();
    return sf::Vector2i(i % w, std::ceil(i / w));
}

sf::IpAddress World::dimToIp(sf::Vector2i vec, int w)
{
    return sf::IpAddress(vec.x + (w * vec.y));
}

std::vector<std::string> World::explode(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
}
