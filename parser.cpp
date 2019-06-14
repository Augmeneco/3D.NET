#include <regex>

#include "nlohmann/json.hpp"

#include "components/transform.h"
#include "components/model.h"
#include "client.h"

#include "parser.h"



using json = nlohmann::json;

Parser::Parser()
{
    //ctor
}

Parser::~Parser()
{
    //dtor
}

Object* Parser::parseResult(std::string& result, sf::IpAddress ip)
{
    auto sceneJson = json::parse(result);

    Object* rootObj = new Object();

    for(auto childNode : sceneJson["childs"])
    {
        Parser::jsonObjToObjects(childNode, rootObj, ip);
    }

    return rootObj;
}

void Parser::jsonObjToObjects(auto node, Object* parentObj, sf::IpAddress ip)
{
    Object* obj = new Object();

    if (node["tag"] != "root")
    {
        obj->setParent(parentObj);

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
            if (node["components"]["model"].find("uri") != node["components"]["model"].end())
            {
                Client* client = Client::getInstance();
                std::regex uriRegex("^([^:\\/?#]+):?\\/\\/([^\\/?#:]*)?:?(\\d{0,5})(\\/[^?#]*)\\?\?([^#]*)?#?(.*)?");
                std::smatch uriParts;
                std::string uri = node["components"]["model"]["uri"];
                if(std::regex_search(uri, uriParts, uriRegex)) {
                    //uri = node["components"]["model"]["uri"];
                }
                else
                {
                    if (uri.find("./") == 0)
                        uri = "3dnet://" + ip.toString() + "/" + uri.substr(2, uri.size()-2);
                    else if (uri.find("/") == 0)
                        uri = "3dnet://" + ip.toString() + uri;
                    else
                        uri = "3dnet://" + ip.toString() + "/" + uri;
                }
                puts(uri.c_str());
                Client::Response response = client->send(uri);
                std::string objText = response.getText();
                obj->model->loadOBJ(objText);
            }
            else if (node["components"]["model"].find("verticies") != node["components"]["model"].end())
                for (auto vertex : node["components"]["model"]["verticies"])
                {
                    obj->model->verticies.push_back(std::array<float, 3>{{vertex[0], vertex[1], vertex[2]}});
                    //printf("%g, %g, %g\n", (float)vertex[0], (float)vertex[1], (float)vertex[2]);
                }
            if (node["components"]["model"].find("texture_uri") != node["components"]["model"].end())
            {
                Client* client = Client::getInstance();
                std::regex uriRegex("^([^:\\/?#]+):?\\/\\/([^\\/?#:]*)?:?(\\d{0,5})(\\/[^?#]*)\\?\?([^#]*)?#?(.*)?");
                std::smatch uriParts;
                std::string uri = node["components"]["model"]["texture_uri"];
                if(std::regex_search(uri, uriParts, uriRegex)) {
                    //uri = node["components"]["model"]["uri"];
                }
                else
                {
                    if (uri.find("./") == 0)
                        uri = "3dnet://" + ip.toString() + "/" + uri.substr(2, uri.size()-2);
                    else if (uri.find("/") == 0)
                        uri = "3dnet://" + ip.toString() + uri;
                    else
                        uri = "3dnet://" + ip.toString() + "/" + uri;
                }
                Client::Response response = client->send(uri);
                //puts(response.getText().c_str());

                // !!!!!!!!!! NOT THREAD SAFE !!!!!!!!!!!!

                obj->model->textureImage.loadFromMemory(&response.getBinary()[0], response.getBinary().size());
                obj->model->textureLoaded = false;

                //obj->model->texture.loadFromMemory(&response.getBinary()[0], response.getBinary().size());
                //obj->model->texture.copyToImage().saveToFile("./file.png");
            }
        }
    }

    for(auto childNode : node["childs"])
    {
        Parser::jsonObjToObjects(childNode, obj, ip);
    }
}
