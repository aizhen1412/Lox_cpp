#ifndef ERROE_H
#define ERROE_H

#include "token.h"
#include "runtime_error.h"

static bool had_error = false;
static bool hadRuntimeError = false;

class Error
{
public:
    static void ErrorFind(int line, std::string message);
    static void ErrorFind(Token token, std::string message);
    static void RuntimeError(RuntimeError error);

private:
    static void Report(int line, std::string where, std::string message);
};

#endif // ERROE_H