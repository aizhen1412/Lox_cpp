#include <iostream>
#include "runtime_error.h"

RuntimeError::RuntimeError(Token token, std::string message) : token(token), message(message)
{
    std::cout << message << std::endl;
}
