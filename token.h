/*
 * token.h
 * This file defines the Token class, which represents a lexical token in the source code.
 * Each Token has a type, a lexeme, a literal value, and the line number where it was found in the source code.
 * The Token class includes a method for converting a Token to a string for debugging purposes.
 */
#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include "token_type_enum.h"
#include "token_type_functions.h"

class LoxCallable;
class LoxClass;
class LoxInstance;

#define Object std::variant<double, bool, std::string, std::nullptr_t, LoxCallable *, LoxClass *, LoxInstance *>

class Token
{
public:
    Token() = default;
    Token(TokenType type, const std::string lexeme, Object literal, int line);
    std::string ToString() const;

    TokenType type;     // the type of the token
    std::string lexeme; // the lexeme of the token
    Object literal;     // the literal value of the token
    int line;           // the line number where the token was found in the source code
};

#endif // TOKEN_H