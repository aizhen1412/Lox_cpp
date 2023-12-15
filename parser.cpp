#include <stdexcept>
#include <vector>
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
Stmt *Parser::whileStatement()
{
    Consume(LEFT_PAREN, "Expect '(' after 'while'.");
    Expr *condition = Expression_method();
    Consume(RIGHT_PAREN, "Expect ')' after condition.");
    Stmt *body = statement();

    return new While(condition, body);
}
Stmt *Parser::expressionStatement()
{
    Expr *expr = Expression_method();
    Consume(SEMICOLON, "Expect ';' after expression.");
    return new Expression(expr);
}
Function *Parser::function(std::string kind)
{
    Token name = Consume(IDENTIFIER, "Expect " + kind + " name.");
    Consume(LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!Check(RIGHT_PAREN))
    {
        do
        {
            if (parameters.size() >= 255)
            {
                error(Peek(), "Can't have more than 255 parameters.");
            }

            parameters.push_back(Consume(IDENTIFIER, "Expect parameter name."));
        } while (Match(COMMA));
    }
    Consume(RIGHT_PAREN, "Expect ')' after parameters.");
    Consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<Stmt *> body = block();
    return new Function(name, parameters, body);
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
    Expr *expr = or_me();

    if (Match(EQUAL))
    {
        Token equals = Previous();
        Expr *value = assignment();

        if (auto variableExpr = dynamic_cast<Variable *>(expr))
        {
            Token name = variableExpr->name;
            return new Assign(name, value);
        }
        else if (auto getExpr = dynamic_cast<Get *>(expr))
        {
            // Get get = (Get)getExpr;
            return new Set(getExpr->object, getExpr->name, value);

            error(equals, "Invalid assignment target.");
        }
    }            // return expr;
    return expr; //}这样是错的，搞了我三天没找到!!!
}
Expr *Parser::or_me()
{
    Expr *expr = and_me();

    while (Match(OR))
    {
        Token op = Previous();
        Expr *right = and_me();
        expr = new Logical(expr, op, right);
    }

    return expr;
}
Expr *Parser::and_me()
{
    Expr *expr = Equality();

    while (Match(AND))
    {
        Token op = Previous();
        Expr *right = Equality();
        expr = new Logical(expr, op, right);
    }

    return expr;
}
Stmt *Parser::statement()
{
    if (Match(FOR))
        return forStatement();
    if (Match(IF))
        return ifStatement();
    if (Match(PRINT))
        return printStatement();
    if (Match(RETURN))
        return returnStatement();
    if (Match(WHILE))
        return whileStatement();
    if (Match(LEFT_BRACE))
        return new Block(block());

    return expressionStatement();
}
Stmt *Parser::returnStatement()
{
    Token keyword = Previous();
    Expr *value = nullptr;
    if (!Check(SEMICOLON))
    {
        value = Expression_method();
    }

    Consume(SEMICOLON, "Expect ';' after return value.");
    return new Return(keyword, value);
}
Stmt *Parser::forStatement()
{
    Consume(LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt *initializer;
    if (Match(SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (Match(VAR))
    {
        initializer = varDeclaration();
    }
    else
    {
        initializer = expressionStatement();
    }
    Expr *condition = nullptr;
    if (!Check(SEMICOLON))
    {
        condition = Expression_method();
    }
    Consume(SEMICOLON, "Expect ';' after loop condition.");
    Expr *increment = nullptr;
    if (!Check(RIGHT_PAREN))
    {
        increment = Expression_method();
    }
    Consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    Stmt *body = statement();
    if (increment != nullptr)
    {
        std::vector<Stmt *> stmtVector;
        stmtVector.push_back(body);
        stmtVector.push_back(new Expression(increment));
        body = new Block(stmtVector);
    }
    if (condition == nullptr)
        condition = new Literal(true);
    body = new While(condition, body);
    if (initializer != nullptr)
    {
        std::vector<Stmt *> stmtVector;
        stmtVector.push_back(initializer);
        stmtVector.push_back(body);
        body = new Block(stmtVector);
    }
    return body;
}
Stmt *Parser::ifStatement()
{
    Consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expr *condition = Expression_method();
    Consume(RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt *thenBranch = statement();
    Stmt *elseBranch = nullptr;
    if (Match(ELSE))
    {
        elseBranch = statement();
    }

    return new If(condition, thenBranch, elseBranch);
}
Stmt *Parser::declaration()
{
    try
    {
        if (Match(CLASS))
            return classDeclaration();
        if (Match(FUN))
            return function("function");
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
Stmt *Parser::classDeclaration()
{
    Token name = Consume(IDENTIFIER, "Expect class name.");
    Variable *superclass = nullptr;
    if (Match(LESS))
    {
        Consume(IDENTIFIER, "Expect superclass name.");
        superclass = new Variable(Previous());
    }
    Consume(LEFT_BRACE, "Expect '{' before class body.");
    std::vector<Function *> methods;
    while (!Check(RIGHT_BRACE) && !IsAtEnd())
    {
        methods.push_back(function("method"));
    }
    Consume(RIGHT_BRACE, "Expect '}' after class body.");

    return new Class(name, superclass, methods);
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

    return call();
}
Expr *Parser::finishCall(Expr *callee)
{
    std::vector<Expr *> arguments;
    if (!Check(RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 255)
            {
                error(Peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(Expression_method());
        } while (Match(COMMA));
    }

    Token paren = Consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return new Call(callee, paren, arguments);
}
Expr *Parser::call()
{
    Expr *expr = Primary();

    while (true)
    {
        if (Match(LEFT_PAREN))
        {
            expr = finishCall(expr);
        }
        else if (Match(DOT))
        {
            Token name = Consume(IDENTIFIER, "Expect property name after '.'.");
            expr = new Get(expr, name);
        }
        else
        {
            break;
        }
    }

    return expr;
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
    if (Match(SUPER))
    {
        Token keyword = Previous();
        Consume(DOT, "Expect '.' after 'super'.");
        Token method = Consume(IDENTIFIER,
                               "Expect superclass method name.");
        return new Super(keyword, method);
    }
    if (Match(THIS))
        return new This(Previous());
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
