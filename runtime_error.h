#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include "token.h"

class RuntimeError : public std::exception
{
public:
    RuntimeError(Token token, std::string message) : token(token), message(message)
    {
        std::cout << message << std::endl;
    }
    Token token;
    std::string message;
};
#endif // RUNTIME_ERROR_H