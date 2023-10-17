#include <stdexcept>
#include "parser.h"
#include "error.h"

#include <iostream>

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
}

std::vector<Stmt *> Parser::parse() // 解析表达式
{
    // try
    // {
    //     return Expression();
    // }
    // catch (ParseError error)
    // {
    //     return nullptr;
    // }
    std::vector<Stmt *> statements;
    while (!IsAtEnd())
    {
        statements.push_back(declaration());    
    }

    return statements;
}

Expr *Parser::Expression_method()
{
    return assignment();
    //  return Equality(); // 等式
}
Stmt *Parser::printStatement()
{
    Expr *value = Expression_method();
    Consume(SEMICOLON, "Expect ';' after value.");
    return new Print(value);
}
Stmt *Parser::varDeclaration()
{
    Token name = Consume(IDENTIFIER, "Expect variable name.");

    Expr *initializer = nullptr;
    if (Match(EQUAL))
    {
        initializer = Expression_method();
    }

    Consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new Var(name, initializer);
}
Stmt *Parser::expressionStatement()
{
    Expr *expr = Expression_method();
    Consume(SEMICOLON, "Expect ';' after expression.");
    return new Expression(expr);
}
std::vector<Stmt *> Parser::block()
{
    std::vector<Stmt *> statements;

    while (!Check(RIGHT_BRACE) && !IsAtEnd())
    {
        statements.push_back(declaration());
    }

    Consume(RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}

Expr *Parser::assignment()
{
    Expr *expr = Equality();

    if (Match(EQUAL))
    {
        Token equals = Previous();
        Expr *value = assignment();

        if (auto variableExpr = dynamic_cast<Variable *>(expr))
        {
            Token name = variableExpr->name;
            return new Assign(name, value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}
Stmt *Parser::statement()
{
    if (Match(PRINT))
        return printStatement();
    if (Match(LEFT_BRACE))
        return new Block(block());

    return expressionStatement();
}
Stmt *Parser::declaration()
{
    try
    {
        if (Match(VAR))
            return varDeclaration();

        return statement();
    }
    catch (ParseError error)
    {
        // synchronize();
        return nullptr;
    }
}
Expr *Parser::Equality()
{
    Expr *expr = Comparison();

    while (Match(BANG_EQUAL, EQUAL_EQUAL))
    {
        Token op = Previous(); // 操作符
        Expr *right = Comparison();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr *Parser::Comparison()
{
    Expr *expr = Term();

    while (Match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
    {
        Token op = Previous();
        Expr *right = Term();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr *Parser::Term()
{
    Expr *expr = Factor();

    while (Match(MINUS, PLUS))
    {
        Token op = Previous();
        Expr *right = Factor();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr *Parser::Factor()
{
    Expr *expr = Unary();

    while (Match(SLASH, STAR))
    {
        Token op = Previous();
        Expr *right = Unary();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr *Parser::Unary()
{
    if (Match(BANG, MINUS))
    {
        Token op = Previous();
        Expr *right = Unary();
        return new class Unary(op, right);
    }

    return Primary();
}

Expr *Parser::Primary()
{
    if (Match(FALSE))
        return new Literal(false);
    if (Match(TRUE))
        return new Literal(true);
    if (Match(NIL))
        return new Literal(nullptr);

    if (Match(NUMBER, STRING))
    {
        return new Literal(Previous().literal);
    }
    if (Match(IDENTIFIER))
    {
        return new Variable(Previous());
    }
    if (Match(LEFT_PAREN))
    {
        Expr *expr = Expression_method();
        Consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }
    throw error(Peek(), "Expect expression.");
    // exit(65);
}

template <typename... Args>
bool Parser::Match(Args... types)
{
    TokenType typesArray[] = {types...};

    for (TokenType type : typesArray)
    {
        if (Check(type))
        {
            Advance();
            return true;
        }
    }
    return false;
}

Token Parser::Consume(TokenType type, std::string message)
{
    if (Check(type))
        return Advance();

    throw error(Peek(), message);
    // exit(65);
}

Parser::ParseError Parser::error(Token token, std::string message)
{
    Error::ErrorFind(token, message);
    return ParseError();
}

void Parser::Synchronize()
{
    Advance();

    while (!IsAtEnd())
    {
        if (Previous().type == SEMICOLON)
            return;

        switch (Peek().type)
        {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        }

        Advance();
    }
}

bool Parser::Check(TokenType type)
{
    if (IsAtEnd())
        return false;
    return Peek().type == type;
}

Token Parser::Advance()
{
    if (!IsAtEnd())
        current++;
    return Previous();
}

bool Parser::IsAtEnd()
{
    return Peek().type == END_OF_FILE;
}

Token Parser::Peek()
{
    if (current >= tokens.size())
    {
        return Token(END_OF_FILE, "", nullptr, 0);
    }
    return tokens[current];
}

Token Parser::Previous()
{
    return tokens[current - 1];
}
