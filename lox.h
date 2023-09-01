#ifndef LOX_H
#define LOX_H

#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "token.h"
#include "scanner.h"

static bool had_error = false;

class Lox
{
public:
    static void RunFile(const std::string &filePath)
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
    }

    static void RunPrompt()
    {
        std::string line;

        while (true)
        {
            std::cout << "> ";
            std::getline(std::cin, line);
            if (std::cin.eof() || line.empty())
            {
                break;
            }
            Run(line);
            had_error = false;
        }
    }
    static void Error(int line, std::string message)
    {
        Report(line, "", message);
    }

private:
    static void Run(const std::string &source)
    {
        Scanner scanner(source);
        std::vector<Token> tokens = scanner.scanTokens();

        // For now, just print the tokens.
        for (const Token &token : tokens)
        {
            std::cout << token << std::endl;
        }
    }
    static void Report(int line, std::string where, std::string message)
    {
        std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
        had_error = true;
    }
};
#endif // LOX_H