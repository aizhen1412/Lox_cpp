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

    Resolver(Interpreter *interpreter);

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
    void Resolve(std::vector<Stmt *> statements);
    void Resolve(Stmt *stmt);
    void Resolve(Expr *expr);
    void ResolveFunction(Function *function, FunctionType type);
    void BeginScope();
    void EndScope();
    void Declare(const Token &name);
    void Define(Token &name);
    void ResolveLocal(Expr *expr, const Token &name);

    ClassType currentClass = ClassType::NONE_CLASS;

    Interpreter *interpreter;
    std::stack<std::map<std::string, bool>> scopes;
    FunctionType currentFunction = FunctionType::NONE;
};
#endif // RESOLVER_H