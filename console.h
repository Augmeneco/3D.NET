#ifndef CONSOLE_H
#define CONSOLE_H
#include <string>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

class Console
{
public:
    Console();
    ~Console();

    void init();
    static bool handleInput(Console* console);

protected:

private:
};

#endif // CONSOLE_H
