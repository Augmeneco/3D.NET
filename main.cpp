#include "program.h"

int main()
{
    Program* program = Program::getInstance();
    program->run();
    return 0;
}
