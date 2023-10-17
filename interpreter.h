#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <memory>
#include "ast_printer.h"
#include "expr.h"
#include "environment.h"

class Interpreter : AstPrinter // 后面换成visitor
{
public:
    Object VisitLiteral(Literal &expr) override;
    Object VisitUnary(Unary &expr) override;
    Object VisitVariable(Variable &expr) override;
    Object VisitGrouping(Grouping &expr) override;
    Object VisitBinary(Binary &expr) override;
    void CheckNumberOperand(Token op, Object operand);
    void CheckNumberOperands(Token op, Object left, Object right);
    bool IsTruthy(Object object);
    bool IsEqual(Object a, Object b);
    Object Evaluate(Expr *expr);
    void execute(Stmt *stmt);
    void executeBlock(std::vector<Stmt *> statements, Environment *environment);
    Object visitBlockStmt(Block &stmt) override;
    Object visitExpressionStmt(Expression &stmt) override;
    Object visitPrintStmt(Print &stmt) override;
    Object visitVarStmt(Var &stmt) override;
    Object VisitAssignExpr(Assign &expr) override;
    void Interpret(std::vector<Stmt *> statements);
    std::string Stringify(Object object);

private:
    Environment *environment = new Environment();
};

#endif // INTERPRETER_H