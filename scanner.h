#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"
#include "token_type_enum.h"

class Scanner
{
public:
    Scanner(std::string source);
    std::vector<Token> ScanTokens();

private:
    const std::string source;  // 源代码
    std::vector<Token> tokens; // 词法单元列表
    unsigned start = 0;        // 当前词法单元的起始位置
    unsigned current = 0;      // 当前词法单元的结束位置
    unsigned line = 1;         // 当前词法单元所在的行号

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

    void ScanToken();
    char Peek();
    char PeekNext();
    char Advance();
    void AddToken(TokenType type);
    void AddToken(TokenType type, Object literal);
    void Number();
    void Identifier();
    void String();
    bool IsDigit(char c);
    bool Match(const char expected);
    bool IsAlpha(char c);
    bool IsAlphaNumeric(char c);
    bool IsAtEnd();
};
#endif // SCANNER_H