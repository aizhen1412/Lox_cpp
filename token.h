#ifndef TOKEN_H
#define TOKEN_H

#include "token_type.h"
#include <string>
#include <any>

class Token // 词法单元
{
public:
    Token(TokenType type, std::string lexeme, std::any literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line)
    {
        // 构造函数体为空，因为成员变量已经在初始化列表中初始化
    }
    std::string ToString() // 将词法单元转换为字符串
    {
        return type + " " + lexeme + " " + std::any_cast<std::string>(literal);
    }

private:
    const TokenType type;
    const std::string lexeme;
    const std::any literal; // 词法单元的字面量
    const int line;
};
#endif // TOKEN_H