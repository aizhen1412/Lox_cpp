#include <iostream>
#include <variant>
#include "token.h"
#include "token_type_enum.h"
#include "token_type_functions.h"

Token::Token(TokenType type, const std::string lexeme, Object literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line)
{
    // 构造函数体为空，因为成员变量已经在初始化列表中初始化
}

std::string Token::ToString() const
{
    if (std::holds_alternative<double>(literal))
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

    return TokenType2String(type) + " " + lexeme + " ";
}