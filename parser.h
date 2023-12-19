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
    void Synchronize();
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

    Token Consume(TokenType type, std::string message);
    ParseError Error(Token token, std::string message);
    bool Check(TokenType type);
    Token Advance();
    bool IsAtEnd();
    Token Peek();
    Token Previous();

    int current = 0;
    std::vector<Token> tokens;
};

#endif // PARSER_H