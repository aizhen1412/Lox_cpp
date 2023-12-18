#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <memory>
#include "ast_printer.h"
#include "expr.h"
#include "environment.h"
#include "lox_function.h"
#include "error.h"
#include "lox_class.h"

class Interpreter : public Visitor // 后面换成visitor
{
public:
    int count = 0;
    Interpreter() = default;

    Object VisitGetExpr(Get &Expr) override;
    Object VisitSuperExpr(Super &Expr) override;
    Object VisitLiteralExpr(Literal &expr) override;
    Object VisitLogicalExpr(Logical &expr) override;
    Object VisitSetExpr(Set &Expr) override;
    Object VisitThisExpr(This &expr);
    Object VisitUnaryExpr(Unary &expr) override;
    Object VisitVariableExpr(Variable *expr) override;
    Object VisitGroupingExpr(Grouping &expr) override;
    Object VisitBinaryExpr(Binary &expr) override;
    Object VisitCallExpr(Call &expr);
    Object LookUpVariable(Token name, Expr *expr);

    void CheckNumberOperand(Token op, Object operand);
    void CheckNumberOperands(Token op, Object left, Object right);
    bool IsTruthy(Object object);
    bool IsEqual(Object a, Object b);
    Object Evaluate(Expr *expr);
    void Execute(Stmt *stmt);
    void Resolve(Expr *expr, int depth);
    void ExecuteBlock(std::vector<Stmt *> statements, Environment *environment);
    void Interpret(std::vector<Stmt *> statements);
    std::string Stringify(Object object);

    Object VisitBlockStmt(Block &stmt) override;
    Object VisitClassStmt(Class &stmt) override;
    Object VisitExpressionStmt(Expression &stmt) override;
    Object VisitFunctionStmt(Function &stmt);
    Object VisitIfStmt(If &stmt);
    Object VisitPrintStmt(Print &stmt) override;
    Object VisitReturnStmt(Return &stmt);
    Object VisitVarStmt(Var &stmt) override;
    Object VisitWhileStmt(While &stmt) override;
    Object VisitAssignExpr(Assign &expr);

    Environment *globals = new Environment();
    Environment *environment = globals;
    std::unordered_map<Expr *, int> locals;
};

#endif // INTERPRETER_H