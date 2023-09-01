#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include "token.h"
#include "token_type.h"
#include "lox.h"

class Scanner
{
public:
    Scanner(std::string source) : source(source)
    {
        // 构造函数体为空，因为成员变量已经在初始化列表中初始化
    }
    std::vector<Token> ScanTokens()
    {
        while (!IsAtEnd())
        {
            // We are at the beginning of the next lexeme.
            start = current;
            ScanToken();
        }

        return tokens;
    }

private:
    const std::string source;  // 源代码
    std::vector<Token> tokens; // 词法单元列表
    int start = 0;             // 记录当前词法单元的起始位置
    int current = 0;           // 记录当前词法单元的结束位置
    int line = 1;              // 记录当前词法单元所在的行号

    bool IsAtEnd() // 判断是否到达末尾
    {
        return current >= source.length();
    }

    char advance() // 读取下一个字符
    {
        current++;
        return source[current - 1];
    }
    void addToken(TokenType type) // 添加词法单元
    {
        addToken(type, nullptr);
    }
    void addToken(TokenType type, std::any literal) // 添加词法单元
    {
        // std::string text = source.substring(start, current); // 从源代码中截取出当前词法单元的字符串
        std::string text = source.substr(start, current - start); // 从源代码中截取出当前词法单元的字符串
        tokens.push_back(Token(type, text, literal, line));       // 将当前词法单元添加到词法单元列表中
    }
    void ScanToken() // 词法分析
    {
        char c = advance();
        switch (c)
        {
        case '(':
            addToken(LEFT_PAREN);
            break;
        case ')':
            addToken(RIGHT_PAREN);
            break;
        case '{':
            addToken(LEFT_BRACE);
            break;
        case '}':
            addToken(RIGHT_BRACE);
            break;
        case ',':
            addToken(COMMA);
            break;
        case '.':
            addToken(DOT);
            break;
        case '-':
            addToken(MINUS);
            break;
        case '+':
            addToken(PLUS);
            break;
        case ';':
            addToken(SEMICOLON);
            break;
        case '*':
            addToken(STAR);
            break;
        default:
            Lox::Error(line, "Unexpected character.");
            break;
        }
    }
};
#endif // SCANNER_H