#include <sstream>
#include <string.h>
#include <iostream>
#include <utility>
#include "program.h"
#include "console.h"


Console::Console()
{

}

Console::~Console()
{

}

void Console::init()
{
    Program* program = Program::getInstance();
    auto panel = tgui::Panel::create({"100%", 300});
    panel->setEnabled(false);
    panel->setVisible(false);

    auto outputRender = tgui::TextBox::create();
    outputRender->setPosition({0, 0});
    outputRender->setSize({"100%", 283});
    outputRender->setReadOnly(true);
    panel->add(outputRender, "OutputTextBox");

    auto inputRender = tgui::EditBox::create();
    inputRender->setPosition({0, 283});
    inputRender->setSize({"100%", 17});
    inputRender->connect("ReturnKeyPressed", this->handleInput, this);
    panel->add(inputRender, "InputEditBox");

    program->gui.add(panel, "ConsolePanel");
}

bool Console::handleInput(Console* console)
{
    Program* program = Program::getInstance();
    auto panel = program->gui.get<tgui::Panel>("ConsolePanel");
    auto output = panel->get<tgui::TextBox>("OutputTextBox");
    auto input = panel->get<tgui::EditBox>("InputEditBox");

    std::vector<std::string> parsedInput;
    std::istringstream iss(input->getText());

    for (std::string token; std::getline(iss, token, ' '); )
    {
        parsedInput.push_back(std::move(token));
    }

    output->addText("> "+input->getText()+"\n");
    input->setText("");
    if(parsedInput[0] == "tp_chunk")
    {
        program->addSceneToLoad(sf::Vector2i(std::stoi(parsedInput[1]), std::stoi(parsedInput[2])));
        for (int x = std::stoi(parsedInput[1]) - 3; x < std::stoi(parsedInput[1]) + 3; x++)
            for (int y = std::stoi(parsedInput[2]) - 3; y < std::stoi(parsedInput[2]) + 3; y++)
                if ((x >= 0) && (x <= 65565) &&
                    (y >= 0) && (y <= 65565))
                    program->addSceneToLoad(sf::Vector2i(x, y));
        program->camera.activeChunk.x = std::stoi(parsedInput[1]);
        program->camera.activeChunk.y = std::stoi(parsedInput[2]);
        program->camera.transform.x = (program->camera.activeChunk.x * -20 - 10 - program->camera.renderingCoords.x) / -1.0;
        program->camera.transform.z = (program->camera.activeChunk.y * -20 - 10 - program->camera.renderingCoords.z) / -1.0;
    }
    else if (parsedInput[0] == "max_fps")
    {
        program->maxFPS = std::stoi(parsedInput[1]);
    }
    else
    {
        output->addText("Unknown command\n");
    }
}
