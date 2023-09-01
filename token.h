#ifndef TOKEN_H
#define TOKEN_H

#include "token_type.h"
#include <string>

class Token
{
public:
    Token(TokenType type, std::string lexeme, Object literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line)
    {
        // 构造函数体为空，因为成员变量已经在初始化列表中初始化
    }
    std::string ToString()
    {
        return type + " " + lexeme + " " + literal;
    }

private:
    const TokenType type;
    const std::string lexeme;
    const Object literal;
    const int line;
}
#endif // TOKEN_H