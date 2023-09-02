#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include "token.h"
#include "token_type.h"

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
        case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;
        case '/':
            if (match('/'))
            {

                while (peek() != '\n' && !IsAtEnd())
                    advance();
            }
            else
            {
                addToken(SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n':
            line++;
            break;
        case '"':
            string();
            break;
        default:
            if (isDigit(c))
            {
                number();
            }
            else if (isAlpha(c))
            {
                identifier();
            }
            else
            {
                // Lox::Error(line, "Unexpected character.");
                std::cout << "Unexpected character." << std::endl;
            }
            break;
        }
    }
    void identifier()
    {
        while (isAlphaNumeric(peek()))
            advance();

        std::string text = source.substr(start, current); // 从源代码中截取出当前词法单元的字符串
        // keywords["default"] = TokenType::FAULT;
        //  TokenType type = keywords[text];
        //  if (type ==) // 如果当前词法单元不是关键字，则将其类型设置为标识符
        //      type = IDENTIFIER;
        TokenType type;
        auto it = keywords.find(text);

        if (it != keywords.end())
        {
            type = it->second;
            // 处理已找到的情况
        }
        else
        {
            type = TokenType::IDENTIFIER;
            // 处理未找到的情况
        }

        addToken(type);
    }
    void number()
    {
        while (isDigit(peek()))
            advance();

        // Look for a fractional part.
        if (peek() == '.' && isDigit(peekNext()))
        {
            // Consume the "."
            advance();

            while (isDigit(peek()))
                advance();
        }

        addToken(NUMBER,
                 std::stod(source.substr(start, current)));
    }
    bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }
    void string()
    {
        while (peek() != '"' && !IsAtEnd())
        {
            if (peek() == '\n')
                line++;
            advance();
        }

        if (IsAtEnd())
        {
            // Lox::Error(line, "Unterminated string.");
            std::cout << "Unterminated string." << std::endl;
            return;
        }

        // The closing ".
        advance();

        // Trim the surrounding quotes.
        std::string value = source.substr(start + 1, current - 1);
        addToken(STRING, value);
    }
    bool match(char expected)
    {
        if (IsAtEnd())
            return false;
        if (source[current] != expected)
            return false;

        current++;
        return true;
    }

    char peek()
    {
        if (IsAtEnd())
            return '\0';
        return source[current];
    }
    char peekNext()
    {
        if (current + 1 >= source.length())
            return '\0';
        return source[current + 1];
    }
    bool isAlpha(char c)
    {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z') ||
               c == '_';
    }
    bool isAlphaNumeric(char c)
    {
        return isAlpha(c) || isDigit(c);
    }
    std::unordered_map<std::string, TokenType> keywords = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}
        // 其他关键字
    };
};
#endif // SCANNER_H