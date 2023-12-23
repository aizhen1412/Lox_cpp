#ifndef ERROE_H
#define ERROE_H

#include "token.h"
#include "runtime_error.h"

extern bool had_error;
extern bool had_runtime_error;

class Error
{
public:
    static void ReportError(int line, std::string message);
    static void ReportError(Token token, std::string message);
    static void ProcessRuntimeError(RuntimeError error);

private:
    static void Report(int line, std::string where, std::string message);
};

#endif // ERROE_H