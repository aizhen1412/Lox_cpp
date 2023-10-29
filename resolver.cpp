#include "resolver.h"
#include "error.h"

Resolver::Resolver(Interpreter *interpreter)
{
    this->interpreter = interpreter;
}
Object Resolver::visitBlockStmt(Block &stmt)
{
    beginScope();
    resolve(stmt.statements);
    endScope();
    return nullptr;
}
Object Resolver::visitExpressionStmt(Expression &stmt)
{
    resolve(stmt.expression);
    return nullptr;
}
Object Resolver::visitFunctionStmt(Function &stmt)
{
    declare(stmt.name);
    define(stmt.name);
    resolveFunction(&stmt, FunctionType::FUNCTION);
    return nullptr;
}
Object Resolver::visitIfStmt(If &stmt)
{
    resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch != nullptr)
        resolve(stmt.elseBranch);
    return nullptr;
}
Object Resolver::visitPrintStmt(Print &stmt)
{
    resolve(stmt.expression);

    return nullptr;
}
Object Resolver::visitReturnStmt(Return &stmt)
{
    if (currentFunction == FunctionType::NONE)
    {
        Error::ErrorFind(stmt.keyword, "Can't return from top-level code.");
    }
    if (stmt.value != nullptr)
    {
        resolve(stmt.value);
    }

    return nullptr;
}
Object Resolver::visitVarStmt(Var &stmt)
{
    declare(stmt.name);
    if (stmt.initializer != nullptr)
    {
        resolve(stmt.initializer);
    }
    define(stmt.name);
    return nullptr;
}
Object Resolver::visitWhileStmt(While &stmt)
{
    resolve(stmt.condition);
    resolve(stmt.body);
    return nullptr;
}
Object Resolver::VisitAssignExpr(Assign &expr)
{
    resolve(expr.value);
    resolveLocal(&expr, expr.name);
    return nullptr;
}
Object Resolver::VisitBinary(Binary &expr)
{
    resolve(expr.left);
    resolve(expr.right);
    return nullptr;
}
Object Resolver::VisitCall(Call &expr)
{
    resolve(expr.callee);

    for (Expr *argument : expr.arguments)
    {
        resolve(argument);
    }

    return nullptr;
}
Object Resolver::VisitGrouping(Grouping &expr)
{
    resolve(expr.expression);
    return nullptr;
}
Object Resolver::VisitLiteral(Literal &expr)
{
    return nullptr;
}
Object Resolver::VisitLogical(Logical &expr)
{
    resolve(expr.left);
    resolve(expr.right);
    return nullptr;
}
Object Resolver::VisitUnary(Unary &expr)
{
    resolve(expr.right);
    return nullptr;
}
Object Resolver::VisitVariable(Variable *expr)
{
    if (!scopes.empty() && !scopes.top().empty() && (scopes.top()[expr->name.lexeme] == false)) // scopes.top()不为空但map没有找到值会返回默认值bool为flase
    {
        Error::ErrorFind(expr->name, "Can't read local variable in its own initializer.");
    }

    // if ((!scopes.empty()) &&
    //     (scopes.top()[expr->name.lexeme] == false))
    // {
    //     Error::ErrorFind(expr->name, "Can't read local variable in its own initializer.");
    // }

    resolveLocal(expr, expr->name);
    return nullptr;
}
void Resolver::resolve(std::vector<Stmt *> statements)
{
    for (Stmt *statement : statements)
    {
        resolve(statement);
    }
}
void Resolver::resolve(Stmt *stmt)
{
    stmt->accept(*this);
}
void Resolver::resolve(Expr *expr)
{
    expr->Accept(*this);
}
void Resolver::resolveFunction(Function *function, FunctionType type)
{

    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;

    beginScope();
    for (Token param : function->params)
    {
        declare(param);
        define(param);
    }

    resolve(function->body);

    endScope();
    currentFunction = enclosingFunction;
}
void Resolver::beginScope()
{
    std::map<std::string, bool> newScope;
    scopes.push(newScope);
}
void Resolver::endScope()
{
    scopes.pop();
}
void Resolver::declare(const Token &name)
{
    if (scopes.empty())
    {
        return;
    }

    std::map<std::string, bool> &scope = scopes.top();
    auto ret = scope.find(name.lexeme);
    if (ret != scope.end()) //
    {
        Error::ErrorFind(name, "Already variable with this name in this scope.");
    }
    scope[name.lexeme] = false;
}
void Resolver::define(Token &name)
{
    if (scopes.empty())
        return;
    scopes.top()[name.lexeme] = true;
}
void Resolver::resolveLocal(Expr *expr, const Token &name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        const std::map<std::string, bool> &currentScope = scopes.top();
        auto it = currentScope.find(name.lexeme);
        if (it != currentScope.end())
        {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}
