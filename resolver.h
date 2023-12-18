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

    ClassType currentClass = ClassType::NONE_CLASS;

    Interpreter *interpreter;
    std::stack<std::map<std::string, bool>> scopes;
    FunctionType currentFunction = FunctionType::NONE;

    Resolver(Interpreter *interpreter);
    Object visitBlockStmt(Block &stmt) override;
    Object visitClassStmt(Class &stmt);
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

    Object VisitGet(Get &expr); // 应该是这里的问题，interpreter没有Expr这个字母
    Object VisitGrouping(Grouping &expr) override;
    Object VisitLiteral(Literal &expr) override;
    Object VisitLogical(Logical &expr) override;
    Object VisitSet(Set &expr);
    Object VisitSuper(Super &expr)
    {
        if (currentClass == ClassType::NONE_CLASS)
        {
            Error::ErrorFind(expr.keyword,
                             "Can't use 'super' outside of a class.");
        }
        else if (currentClass != ClassType::SUBCLASS)
        {
            Error::ErrorFind(expr.keyword,
                             "Can't use 'super' in a class with no superclass.");
        }
        resolveLocal(&expr, expr.keyword);
        return nullptr;
    }
    Object VisitThis(This &expr) override;
    Object VisitUnary(Unary &expr) override;
    Object VisitVariable(Variable *expr);
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