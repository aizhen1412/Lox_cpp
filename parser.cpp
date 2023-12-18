#include <stdexcept>
#include <vector>
#include "parser.h"
#include "error.h"

#include <iostream>

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
}

std::vector<Stmt *> Parser::Parse() // 解析表达式
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
        statements.push_back(Declaration());
    }

    return statements;
}

Expr *Parser::ExpressionFun()
{
    return Assignment();
    //  return Equality(); // 等式
}
Stmt *Parser::PrintStatement()
{
    Expr *value = ExpressionFun();
    Consume(SEMICOLON, "Expect ';' after value.");
    return new Print(value);
}
Stmt *Parser::VarDeclaration()
{
    Token name = Consume(IDENTIFIER, "Expect variable name.");

    Expr *initializer = nullptr;
    if (Match(EQUAL))
    {
        initializer = ExpressionFun();
    }

    Consume(SEMICOLON, "Expect ';' after variable declaration.");
    return new Var(name, initializer);
}
Stmt *Parser::WhileStatement()
{
    Consume(LEFT_PAREN, "Expect '(' after 'while'.");
    Expr *condition = ExpressionFun();
    Consume(RIGHT_PAREN, "Expect ')' after condition.");
    Stmt *body = Statement();

    return new While(condition, body);
}
Stmt *Parser::ExpressionStatement()
{
    Expr *expr = ExpressionFun();
    Consume(SEMICOLON, "Expect ';' after expression.");
    return new Expression(expr);
}
Function *Parser::FunctionMethod(std::string kind)
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
                Error(Peek(), "Can't have more than 255 parameters.");
            }

            parameters.push_back(Consume(IDENTIFIER, "Expect parameter name."));
        } while (Match(COMMA));
    }
    Consume(RIGHT_PAREN, "Expect ')' after parameters.");
    Consume(LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<Stmt *> body = BlockFun();
    return new Function(name, parameters, body);
}
std::vector<Stmt *> Parser::BlockFun()
{
    std::vector<Stmt *> statements;

    while (!Check(RIGHT_BRACE) && !IsAtEnd())
    {
        statements.push_back(Declaration());
    }

    Consume(RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}

Expr *Parser::Assignment()
{
    Expr *expr = Or();

    if (Match(EQUAL))
    {
        Token equals = Previous();
        Expr *value = Assignment();

        if (auto variableExpr = dynamic_cast<Variable *>(expr))
        {
            Token name = variableExpr->name;
            return new Assign(name, value);
        }
        else if (auto getExpr = dynamic_cast<Get *>(expr))
        {
            // Get get = (Get)getExpr;
            return new Set(getExpr->object, getExpr->name, value);

            Error(equals, "Invalid assignment target.");
        }
    }            // return expr;
    return expr; //}这样是错的，搞了我三天没找到!!!
}
Expr *Parser::Or()
{
    Expr *expr = And();

    while (Match(OR))
    {
        Token op = Previous();
        Expr *right = And();
        expr = new Logical(expr, op, right);
    }

    return expr;
}
Expr *Parser::And()
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
Stmt *Parser::Statement()
{
    if (Match(FOR))
        return ForStatement();
    if (Match(IF))
        return IfStatement();
    if (Match(PRINT))
        return PrintStatement();
    if (Match(RETURN))
        return ReturnStatement();
    if (Match(WHILE))
        return WhileStatement();
    if (Match(LEFT_BRACE))
        return new Block(BlockFun());

    return ExpressionStatement();
}
Stmt *Parser::ReturnStatement()
{
    Token keyword = Previous();
    Expr *value = nullptr;
    if (!Check(SEMICOLON))
    {
        value = ExpressionFun();
    }

    Consume(SEMICOLON, "Expect ';' after return value.");
    return new Return(keyword, value);
}
Stmt *Parser::ForStatement()
{
    Consume(LEFT_PAREN, "Expect '(' after 'for'.");

    Stmt *initializer;
    if (Match(SEMICOLON))
    {
        initializer = nullptr;
    }
    else if (Match(VAR))
    {
        initializer = VarDeclaration();
    }
    else
    {
        initializer = ExpressionStatement();
    }
    Expr *condition = nullptr;
    if (!Check(SEMICOLON))
    {
        condition = ExpressionFun();
    }
    Consume(SEMICOLON, "Expect ';' after loop condition.");
    Expr *increment = nullptr;
    if (!Check(RIGHT_PAREN))
    {
        increment = ExpressionFun();
    }
    Consume(RIGHT_PAREN, "Expect ')' after for clauses.");
    Stmt *body = Statement();
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
Stmt *Parser::IfStatement()
{
    Consume(LEFT_PAREN, "Expect '(' after 'if'.");
    Expr *condition = ExpressionFun();
    Consume(RIGHT_PAREN, "Expect ')' after if condition.");

    Stmt *thenBranch = Statement();
    Stmt *elseBranch = nullptr;
    if (Match(ELSE))
    {
        elseBranch = Statement();
    }

    return new If(condition, thenBranch, elseBranch);
}
Stmt *Parser::Declaration()
{
    try
    {
        if (Match(CLASS))
            return ClassDeclaration();
        if (Match(FUN))
            return FunctionMethod("function");
        if (Match(VAR))
            return VarDeclaration();

        return Statement();
    }
    catch (ParseError error)
    {
        // synchronize();
        return nullptr;
    }
}
Stmt *Parser::ClassDeclaration()
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
        methods.push_back(FunctionMethod("method"));
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
    Expr *expr = UnaryFun();

    while (Match(SLASH, STAR))
    {
        Token op = Previous();
        Expr *right = UnaryFun();
        expr = new Binary(expr, op, right);
    }

    return expr;
}

Expr *Parser::UnaryFun()
{
    if (Match(BANG, MINUS))
    {
        Token op = Previous();
        Expr *right = UnaryFun();
        return new Unary(op, right);
    }

    return CallFun();
}
Expr *Parser::FinishCall(Expr *callee)
{
    std::vector<Expr *> arguments;
    if (!Check(RIGHT_PAREN))
    {
        do
        {
            if (arguments.size() >= 255)
            {
                Error(Peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(ExpressionFun());
        } while (Match(COMMA));
    }

    Token paren = Consume(RIGHT_PAREN, "Expect ')' after arguments.");

    return new Call(callee, paren, arguments);
}
Expr *Parser::CallFun()
{
    Expr *expr = Primary();

    while (true)
    {
        if (Match(LEFT_PAREN))
        {
            expr = FinishCall(expr);
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
        Expr *expr = ExpressionFun();
        Consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }
    throw Error(Peek(), "Expect expression.");
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

    throw Error(Peek(), message);
    // exit(65);
}

Parser::ParseError Parser::Error(Token token, std::string message)
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
