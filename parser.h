#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include "token.h"
#include "expr.h"

class Parser
{
public:
    Parser(std::vector<Token> tokens);
    std::vector<Stmt *> parse();

private:
    int current = 0;
    std::vector<Token> tokens;

    class ParseError
    {
    };
    Expr *Expression_method();

    Stmt *printStatement();
    Stmt *expressionStatement();
    Stmt *statement();

    Expr *Equality();
    Expr *Comparison();
    Expr *Term();
    Expr *Factor();
    Expr *Unary();
    Expr *Primary();

    template <typename... Args>
    bool Match(Args... types);

    Token Consume(TokenType type, std::string message);
    ParseError error(Token token, std::string message);
    void Synchronize();
    bool Check(TokenType type);
    Token Advance();
    bool IsAtEnd();
    Token Peek();
    Token Previous();
};

#endif // PARSER_H