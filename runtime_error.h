#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include <string>
#include "token.h"

class RuntimeError : public std::exception
{
public:
    Token token;
    std::string message;

    RuntimeError(Token token, std::string message);
};

#endif // RUNTIME_ERROR_H