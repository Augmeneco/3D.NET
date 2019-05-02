#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <string>


class tag_exist
{
public:
    std::string tag;
    const char* what() const noexcept;
};

#endif // EXCEPTIONS_H
