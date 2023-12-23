/*
 * main.cpp
 * This is the main entry point for the application.
 * It handles command line arguments and decides whether to run a Lox script from a file or start a REPL.
 *
 * Author: Galle
 * Date: 2023-12-23
 */
#include <iostream>
#include <string>
#include "token.h"
#include "scanner.h"
#include "lox.h"

int main(int argc, char const *argv[])
{
    if (argc > 2)
    {
        std::cerr << "Usage: ./cpplox [script]" << std::endl;
        return 64;
    }
    else if (argc == 2)
    {
        Lox::RunFile(argv[1]);
    }
    else
    {
        Lox::RunPrompt();
        return 0;
    }
}