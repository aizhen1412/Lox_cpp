/*
 * token.cpp
 * This file implements the Token class defined in token.h.
 * The Token class represents a lexical token in the source code.
 * Each Token has a type, a lexeme, a literal value, and the line number where it was found in the source code.
 * The Token class includes a method for converting a Token to a string for debugging purposes.
 */
#include <iostream>
#include <variant>
#include "token.h"
#include "token_type_enum.h"
#include "token_type_functions.h"

Token::Token(TokenType type, const std::string lexeme, Object literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

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