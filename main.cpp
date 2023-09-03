#include <iostream>
#include <string>
#include "token.h"
#include "scanner.h"
#include "lox.h"

int main(int argc, char *argv[])
{
    argc = 0; // test
    if (argc > 1)
    {
        std::cerr << "Usage: ./cpplox [script]" << std::endl;
        return 64;
    }
    else if (argc == 1) // 从文件中读取源代码
    {
        Lox::RunFile(argv[0]);
    }
    else // 命令行
    {
        Lox::RunPrompt();

        return 0;
    }
}