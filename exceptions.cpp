#include "exceptions.h"

const std::string tag_exist::what() const noexcept
{
    return "Тег \"" + this->tag + "\" уже существует";
}
