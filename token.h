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
    Token(){};
    Token(TokenType type, const std::string lexeme, Object literal, int line);
    std::string ToString() const;

    TokenType type;
    std::string lexeme;
    Object literal;
    int line;
};

#endif // TOKEN_H