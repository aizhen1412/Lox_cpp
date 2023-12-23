/*
 * resolver.h
 * This file defines the Resolver class, which is used to resolve and handle the scope of variables and functions in the source code.
 * The Resolver class is a subclass of the Interpreter class, and it overrides the visit methods for each type of statement and expression.
 * The Resolver class includes methods for beginning and ending a scope, declaring and defining a variable, and resolving a local variable.
 */
#ifndef RESOLVER_H
#define RESOLVER_H

#include <stack>
#include <map>
#include "expr.h"
#include "interpreter.h"
#include "error.h"

class Resolver : public Interpreter
{
public:
    Resolver(Interpreter *interpreter);
    void Resolve(std::vector<Stmt *> statements);

private:
    enum FunctionType
    {
        NONE,
        METHOD,
        INITIALIZER,
        FUNCTION
    };
    enum ClassType
    {
        NONE_CLASS,
        CLASS,
        SUBCLASS
    };

    ClassType currentClass = ClassType::NONE_CLASS;
    FunctionType currentFunction = FunctionType::NONE;
    Interpreter *interpreter;
    // A stack of scopes, where each scope is a map from variable names to a boolean value.
    // The boolean value indicates whether the variable has been initialized.
    std::stack<std::map<std::string, bool>> scopes;
    // visitor methods
    Object VisitBlockStmt(Block &stmt) override;
    Object VisitClassStmt(Class &stmt) override;
    Object VisitExpressionStmt(Expression &stmt) override;
    Object VisitFunctionStmt(Function &stmt) override;
    Object VisitIfStmt(If &stmt) override;
    Object VisitPrintStmt(Print &stmt) override;
    Object VisitReturnStmt(Return &stmt) override;
    Object VisitVarStmt(Var &stmt) override;
    Object VisitWhileStmt(While &stmt) override;

    Object VisitAssignExpr(Assign &expr) override;
    Object VisitBinaryExpr(Binary &expr) override;
    Object VisitCallExpr(Call &expr) override;
    Object VisitGetExpr(Get &expr) override;
    Object VisitGroupingExpr(Grouping &expr) override;
    Object VisitLiteralExpr(Literal &expr) override;
    Object VisitLogicalExpr(Logical &expr) override;
    Object VisitSetExpr(Set &expr) override;
    Object VisitSuperExpr(Super &expr) override;
    Object VisitThisExpr(This &expr) override;
    Object VisitUnaryExpr(Unary &expr) override;
    Object VisitVariableExpr(Variable *expr) override;

    void Resolve(Stmt *stmt);
    void Resolve(Expr *expr);
    void ResolveFunction(Function *function, FunctionType type);
    void BeginScope();                                // push a new scope onto the stack
    void EndScope();                                  // pop the current scope off the stack
    void Declare(const Token &name);                  // declare a variable in the current scope
    void Define(Token &name);                         // mark a variable as initialized in the current scope
    void ResolveLocal(Expr *expr, const Token &name); // resolve a local variable
};
#endif // RESOLVER_H