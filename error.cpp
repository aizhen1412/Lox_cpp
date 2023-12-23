/*
 * error.cpp
 * This file implements the Error class defined in error.h.
 * The Error class is used to report errors in the Lox language.
 *
 * The had_error and had_runtime_error flags are global variables that are used to indicate if an error or a runtime error has occurred. They are initially set to false.
 *
 * The ReportError methods take a line or a token and a message, and report an error at the given line or token with the given message. They call the Report method to format and print the error message, and set the had_error flag to true.
 *
 * The ProcessRuntimeError method takes a RuntimeError and reports the error message and the line where the error occurred. It sets the had_runtime_error flag to true.
 *
 * The Report method is a private helper method that formats and prints the error message. It takes a line, a where string that indicates where the error occurred, and a message string that describes the error.
 */
#include <iostream>
#include "error.h"

bool had_error = false;
bool had_runtime_error = false;

void Error::ReportError(int line, std::string message)
{
    Report(line, "", message);
}
void Error::ReportError(Token token, std::string message)
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
void Error::ProcessRuntimeError(RuntimeError error)
{
    std::cout << "[line " + std::to_string(error.Get_token().line) + "] RuntimeError." << std::endl;

    had_runtime_error = true;
}
void Error::Report(int line, std::string where, std::string message) // 报告错误
{
    std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
    had_error = true;
}
