/*
 * runtime_error.cpp
 * This file implements the RuntimeError class defined in runtime_error.h.
 * The RuntimeError class represents a runtime error in the Lox language.
 * Each RuntimeError has a token, which is the token at which the error occurred, and a message, which describes the error.
 * The RuntimeError class includes a constructor for creating a new RuntimeError, and a method for getting the token at which the error occurred.
 */
#include <iostream>
#include "runtime_error.h"

RuntimeError::RuntimeError(Token token, std::string message) : token(token), message(message)
{
    std::cout << message << std::endl;
}
Token RuntimeError::Get_token()
{
    return token;
}