#ifndef LOX_H
#define LOX_H

#include <string>

class Lox
{
public:
    static void RunFile(const std::string &filePath);
    static void RunPrompt();

private:
    static void Run(const std::string &source);
};

#endif // LOX_H