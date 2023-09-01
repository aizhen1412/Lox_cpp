#include <iostream>
#include <string>
#include "token.h"
#include "scanner.h"
#include "lox.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::cerr << "Usage: ./cpplox [script]" << std::endl;
        return 64;
    }
    else if (argc == 1)
    {
        Lox::RunFile(argv[0]);
    }
    else
    {
        Lox::RunPrompt();

        return 0;
    }
} // 4.5.1