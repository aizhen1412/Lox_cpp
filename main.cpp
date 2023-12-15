#include <iostream>
#include <string>
#include "token.h"
#include "scanner.h"
#include "lox.h"

int main(int argc, char const *argv[])
{
    argc = 2; // test
    if (argc > 2)
    {
        std::cerr << "Usage: ./cpplox [script]" << std::endl;
        return 64;
    }
    else if (argc == 2) // 从文件中读取源代码
    {
        Lox::RunFile("E:\\GitRepository\\Lox_cpp\\test.txt");
    }
    else // 命令行
    {
        Lox::RunPrompt();

        return 0;
    }
} // 12.6