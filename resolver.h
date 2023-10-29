#ifndef RESOLVER_H
#define RESOLVER_H

#include <stack>
#include <map>
#include "expr.h"
#include "interpreter.h"

class Resolver : public Interpreter
{
public:
    enum FunctionType
    {
        NONE,
        FUNCTION
    };
    Interpreter *interpreter;
    std::stack<std::map<std::string, bool>> scopes;
    FunctionType currentFunction = FunctionType::NONE;

    Resolver(Interpreter *interpreter);
    Object visitBlockStmt(Block &stmt) override;
    Object visitExpressionStmt(Expression &stmt) override;
    Object visitFunctionStmt(Function &stmt) override;
    Object visitIfStmt(If &stmt) override;
    Object visitPrintStmt(Print &stmt) override;
    Object visitReturnStmt(Return &stmt) override;
    Object visitVarStmt(Var &stmt) override;
    Object visitWhileStmt(While &stmt) override;
    Object VisitAssignExpr(Assign &expr) override;
    Object VisitBinary(Binary &expr) override;
    Object VisitCall(Call &expr) override;
    Object VisitGrouping(Grouping &expr) override;
    Object VisitLiteral(Literal &expr) override;
    Object VisitLogical(Logical &expr) override;
    Object VisitUnary(Unary &expr) override;
    Object VisitVariable(Variable *expr) override;
    void resolve(std::vector<Stmt *> statements);
    void resolve(Stmt *stmt);
    void resolve(Expr *expr);
    void resolveFunction(Function *function, FunctionType type);
    void beginScope();
    void endScope();
    void declare(const Token &name);
    void define(Token &name);
    void resolveLocal(Expr *expr, const Token &name);
};
#endif // RESOLVER_H