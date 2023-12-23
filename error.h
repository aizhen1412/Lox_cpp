/*
 * error.h
 * This file defines the Error class, which is used to report errors in the Lox language.
 * The Error class provides static methods for reporting an error at a specific line, reporting an error at a specific token, and processing a runtime error.
 *
 * The ReportError methods take a line or a token and a message, and report an error at the given line or token with the given message. They set the had_error flag to true.
 *
 * The ProcessRuntimeError method takes a RuntimeError and reports the error message and the line where the error occurred. It sets the had_runtime_error flag to true.
 *
 * The Report method is a private helper method that formats and prints the error message.
 *
 * The had_error and had_runtime_error flags are used to indicate if an error or a runtime error has occurred. They are initially set to false and are set to true when an error occurs.
 */
#ifndef ERROE_H
#define ERROE_H

#include "token.h"
#include "runtime_error.h"

extern bool had_error;
extern bool had_runtime_error;

class Error
{
public:
    // reports an error at a specific line
    static void ReportError(int line, std::string message);
    // reports an error at a specific token
    static void ReportError(Token token, std::string message);
    // processes a runtime error
    static void ProcessRuntimeError(RuntimeError error);

private:
    // formats and prints the error message
    static void Report(int line, std::string where, std::string message);
};

#endif // ERROE_H