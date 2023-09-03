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

static bool had_error = false; // 用于标记是否有错误

class Lox
{
public:
    static void RunFile(const std::string &filePath) // 从文件中读取源代码
    {
        std::ifstream file(filePath); // 打开文件
        if (!file.is_open())          // 判断文件是否打开成功
        {
            std::cerr << "Error opening file: " << filePath << std::endl;
            exit(1);
        }

        std::stringstream buffer; // 缓冲区
        buffer << file.rdbuf();   // 读取文件内容到缓冲区
        Run(buffer.str());
        if (had_error) // 如果有错误，退出程序
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
    static void Run(const std::string &source) // 运行源代码
    {
        Scanner scanner(source);                          // 词法分析
        std::vector<Token> tokens = scanner.ScanTokens(); // 存储词法单元

        // For now, just print the tokens.
        for (const Token &token : tokens)
        {
            std::cout << token.ToString() << std::endl; // 打印词法单元
        }
    }
    static void Report(int line, std::string where, std::string message) // 报告错误
    {
        std::cerr << "[line " << line << "] Error" + where + ": " + message << std::endl;
        had_error = true;
    }
};
#endif // LOX_H