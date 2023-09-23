#ifndef ERROE_H
#define ERROE_H

#include <iostream>
#include "token.h"
#include "runtime_error.h"

static bool had_error = false;
static bool hadRuntimeError = false;

class Error
{
public:
    static void ErrorFind(int line, std::string message)
    {
        Report(line, "", message);
    }
    static void ErrorFind(Token token, std::string message)
    {
        if (token.type == TokenType::END_OF_FILE)
        {
            Report(token.line, " at end", message);
        }
        else
        {
            Report(token.line, " at '" + token.lexeme + "'", message);
        }
    }
    static void RuntimeError(RuntimeError error)
    {
        // std::cout << error.getMessage() + "\n[line " + error.token.line + "]" << std::endl;
        std::cout << "error" << std::endl;
        hadRuntimeError = true;
    }

private:
    static void Report(int line, std::string where, std::string message) // 报告错误
    {
        std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
        had_error = true;
    }
};
#endif // ERROE_H