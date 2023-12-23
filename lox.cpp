/*
 * File: lox.cpp
 * -------------
 * This file implements the Lox class defined in lox.h. The Lox class is the main entry point for the Lox interpreter.
 * It provides methods for running a Lox script from a file or from an interactive prompt.
 *
 * The RunFile method reads a Lox script from a file and executes it. If an error occurs during execution, the program exits with an error code.
 *
 * The RunPrompt method starts an interactive prompt where the user can enter Lox commands, which are executed immediately.
 *
 * The Run method is a private helper method that takes a Lox script as a string and executes it. It performs lexical analysis, parsing, resolution, and interpretation.
 * If an error occurs during any of these stages, it sets the had_error flag and returns immediately.
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lox.h"
#include "token.h"
#include "scanner.h"
#include "error.h"
#include "parser.h"
#include "interpreter.h"
#include "resolver.h"

void Lox::RunFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    Run(buffer.str());

    if (had_error)
        exit(65);
    if (had_runtime_error)
        exit(70);
}

void Lox::RunPrompt()
{
    std::string line;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (std::cin.eof() || line.empty())
            break;

        Run(line);
        had_error = false;
    }
}

void Lox::Run(const std::string &source) // 运行源代码
{
    Scanner scanner(source);                          // 词法分析
    std::vector<Token> tokens = scanner.ScanTokens(); // 存储词法单元

    Parser parser(tokens);
    std::vector<Stmt *> statements = parser.Parse();

    if (had_error)
        return;

    Interpreter interpreter = Interpreter();

    Resolver resolver = Resolver(&interpreter);

    resolver.Resolve(statements);

    if (had_error)
        return;

    interpreter.Interpret(statements);

    for (auto statement : statements)
        delete statement;
}
