#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <memory>
#include "ast_printer.h"
#include "expr.h"
#include "environment.h"

class Interpreter : public Visitor // 后面换成visitor
{
public:
    Interpreter()
    {
        //  globals->define("clock", new LoxCallable());
    }
    Object visitClassStmt(Class &stmt) { return nullptr; };
    Object VisitThis(This &Expr) { return "this"; };
    Object VisitGet(Get &Expr) { return "get"; };
    Object VisitSet(Set &Expr) { return "set"; };
    Object VisitSuper(Super &Expr) { return "super"; };

    Object VisitLiteral(Literal &expr) override;
    Object VisitLogical(Logical &expr) override;
    Object VisitUnary(Unary &expr) override;
    Object VisitVariable(Variable *expr) override;
    Object lookUpVariable(Token name, Expr *expr);
    Object VisitGrouping(Grouping &expr) override;
    Object VisitBinary(Binary &expr) override;
    Object VisitCall(Call &expr);
    void CheckNumberOperand(Token op, Object operand);
    void CheckNumberOperands(Token op, Object left, Object right);
    bool IsTruthy(Object object);
    bool IsEqual(Object a, Object b);
    Object Evaluate(Expr *expr);
    void execute(Stmt *stmt);
    void resolve(Expr *expr, int depth);
    void executeBlock(std::vector<Stmt *> statements, Environment *environment);
    Object visitBlockStmt(Block &stmt) override;
    Object visitExpressionStmt(Expression &stmt) override;
    Object visitFunctionStmt(Function &stmt);
    Object visitIfStmt(If &stmt);
    Object visitPrintStmt(Print &stmt) override;
    Object visitReturnStmt(Return &stmt);
    Object visitVarStmt(Var &stmt) override;
    Object visitWhileStmt(While &stmt) override;
    Object VisitAssignExpr(Assign *expr);
    Object VisitAssignExpr(Assign &expr)
    {
        return nullptr;
    }

    void Interpret(std::vector<Stmt *> statements);
    std::string Stringify(Object object);

    Environment *globals = new Environment();
    Environment *environment = globals;
    std::unordered_map<Expr *, int> locals;
};

#endif // INTERPRETER_H