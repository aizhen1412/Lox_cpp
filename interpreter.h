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
    Interpreter()
    {
        //  globals->define("clock", new LoxCallable());
    }

    Object VisitGet(Get &Expr) override;

    Object VisitSuper(Super &Expr) override;

    Object VisitLiteral(Literal &expr) override;
    Object VisitLogical(Logical &expr) override;
    Object VisitSet(Set &Expr) override;
    Object visitSuperExpr(Super *expr)
    {
        int distance = locals[expr];
        LoxClass *superclass = std::get<LoxClass *>(environment->getAt(distance, "super"));
        LoxInstance *object = std::get<LoxInstance *>(environment->getAt(distance - 1, "this"));
        LoxFunction *method = superclass->findMethod(expr->method.lexeme);
        if (method == nullptr)
        {
            throw new RuntimeError(expr->method,
                                   "Undefined property '" + expr->method.lexeme + "'.");
        }

        return method->bind(object);
    }
    Object VisitThis(This &expr);
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
    Object visitClassStmt(Class &stmt) override;
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