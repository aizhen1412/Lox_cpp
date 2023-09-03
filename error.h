#ifndef ERROE_H
#define ERROE_H
#include <iostream>

static bool had_error = false;

class Error
{
public:
    static void ErrorFind(int line, std::string message)
    {
        Report(line, "", message);
    }

private:
    static void Report(int line, std::string where, std::string message) // 报告错误
    {
        std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
        had_error = true;
    }
};
#endif // ERROE_H