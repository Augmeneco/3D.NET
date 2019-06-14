#ifndef PARSER_H
#define PARSER_H

#include "object.h"
#include "scene.h"

class Parser
{
    public:
        Parser();
        ~Parser();

        static Object* parseResult(std::string& result, sf::IpAddress ip);
        static void jsonObjToObjects(auto sceneUnparsed, Object* parentObj, sf::IpAddress ip);
    protected:

    private:
};

#endif // PARSER_H
