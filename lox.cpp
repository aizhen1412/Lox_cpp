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
}
