#ifndef TOKEN_H
#define TOKEN_H

#define Object std::variant<double, bool, std::string, std::nullptr_t>

#include <string>
#include <variant>
#include <iostream>
#include "token_type.h"

class Token // 词法单元
{
public:
    Token(TokenType type, const std::string lexeme, Object literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line)
    {
        // 构造函数体为空，因为成员变量已经在初始化列表中初始化
    }
    std::string ToString() const // 将词法单元转换为字符串
    {
        if (std::holds_alternative<double>(literal)) // 判断literal的类型
        {
            double literal_value = std::get<double>(literal);
            std::cout << "Stored double: " << literal_value << std::endl;
        }
        else if (std::holds_alternative<std::string>(literal))
        {
            std::string literal_value = std::get<std::string>(literal);
            std::cout << "Stored string: " << literal_value << std::endl;
        }
        else if (std::holds_alternative<std::nullptr_t>(literal))
        {
            std::cout << "Stored nullptr" << std::endl;
        }
        // std::cout << literal.index() << std::endl;
        return TokenType2String(type) + " " + lexeme + " ";
        //     +std::any_cast<std::string>(literal);type +
    }

    TokenType type;     // 词法单元的类型
    std::string lexeme; // 词法单元的字符串
    Object literal;     // 词法单元的字面量
    int line;           // 词法单元所在的行号
};
#endif // TOKEN_H