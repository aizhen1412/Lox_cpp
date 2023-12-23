/*
 * runtime_error.h
 * This file defines the RuntimeError class, which represents a runtime error in the Lox language.
 * Each RuntimeError has a token, which is the token at which the error occurred, and a message, which describes the error.
 * The RuntimeError class includes a constructor for creating a new RuntimeError, and a method for getting the token at which the error occurred.
 */
#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include <string>
#include "token.h"

class RuntimeError : public std::exception
{
public:
    RuntimeError(Token token, std::string message);
    Token Get_token();

private:
    Token token;         // the token at which the error occurred
    std::string message; // the message describing the error
};

#endif // RUNTIME_ERROR_H