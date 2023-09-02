#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <any>
#include <iostream>
#include "token_type.h"

class Token // 词法单元
{
public:
    // friend std::ostream &operator<<(std::ostream &os, const Token &obj);

    Token(TokenType type, std::string lexeme, std::any literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line)
    {
        // 构造函数体为空，因为成员变量已经在初始化列表中初始化
    }
    std::string ToString() const // 将词法单元转换为字符串
    {
        return TokenTypeTOString(type) + " " + lexeme + " ";
        //     +std::any_cast<std::string>(literal);type +
    }

    const TokenType type;     // 词法单元的类型
    const std::string lexeme; // 词法单元的字符串
    const std::any literal;   // 词法单元的字面量
    const int line;           // 词法单元所在的行号
};
// std::ostream &operator<<(std::ostream &os, const Token &obj)
// {
//     os << std::any_cast<std::string>(obj.literal);
//     return os;
// }
#endif // TOKEN_H