/*
 * parser.h
 * This file defines the Parser class, which is used to parse the source code and generate an abstract syntax tree (AST).
 * The Parser class includes methods for parsing different types of statements and expressions, and for handling errors during parsing.
 * The Parser class also includes a nested ParseError class, which represents an error that occurred during parsing.
 */
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
    std::vector<Stmt *> Parse();
    class ParseError
    {
    };

private:
    int current = 0;
    std::vector<Token> tokens;

    void Synchronize(); // if error, skip to the next statement
    Expr *ExpressionFun();
    Stmt *PrintStatement();
    Stmt *VarDeclaration();
    Stmt *WhileStatement();
    Stmt *ExpressionStatement();
    Function *FunctionMethod(std::string kind);
    std::vector<Stmt *> BlockFun();
    Expr *Assignment();
    Expr *Or();
    Expr *And();
    Stmt *Statement();
    Stmt *ReturnStatement();
    Stmt *ForStatement();
    Stmt *IfStatement();
    Stmt *Declaration();
    Stmt *ClassDeclaration();
    Expr *Equality();
    Expr *Comparison();
    Expr *Term();
    Expr *Factor();
    Expr *UnaryFun();
    Expr *FinishCall(Expr *callee);
    Expr *CallFun();
    Expr *Primary();

    template <typename... Args>
    bool Match(Args... types);

    Token Consume(TokenType type, std::string message); // if not match, throw error
    ParseError Error(Token token, std::string message);
    bool Check(TokenType type);
    Token Advance();
    bool IsAtEnd();
    Token Peek();     // return the current token
    Token Previous(); // return the previous token
};

#endif // PARSER_H
