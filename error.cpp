#include <iostream>
#include "error.h"

bool had_error = false;
bool had_runtime_error = false;

void Error::ErrorFind(int line, std::string message)
{
    Report(line, "", message);
}
void Error::ErrorFind(Token token, std::string message)
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
void Error::RuntimeError(class RuntimeError error)
{
    // std::cout << error.getMessage() + "\n[line " + error.token.line + "]" << std::endl;
    std::cout << "error" << std::endl;
    had_runtime_error = true;
}
void Error::Report(int line, std::string where, std::string message) // 报告错误
{
    std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
    had_error = true;
}
