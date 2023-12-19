#include "resolver.h"
#include "error.h"
#include "lox_instance.h"
#include "interpreter.h"

Resolver::Resolver(Interpreter *interpreter)
{
    this->interpreter = interpreter;
}
Object Resolver::VisitBlockStmt(Block &stmt)
{
    BeginScope();
    Resolve(stmt.statements);
    EndScope();
    return nullptr;
}
Object Resolver::VisitClassStmt(Class &stmt)
{
    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;
    Declare(stmt.name);
    Define(stmt.name);
    if (stmt.superclass != nullptr && stmt.name.lexeme == stmt.superclass->name.lexeme)
    {
        Error::ErrorFind(stmt.superclass->name, "A class can't inherit from itself.");
    }
    if (stmt.superclass != nullptr)
    {
        currentClass = ClassType::SUBCLASS;
        Resolve(stmt.superclass);
    }
    if (stmt.superclass != nullptr)
    {
        BeginScope();
        scopes.top()["super"] = true;
    }
    BeginScope();
    scopes.top()["this"] = true;
    for (Function *method : stmt.methods)
    {
        FunctionType declaration = FunctionType::METHOD;
        if (method->name.lexeme.compare("init") == 0)
        {
            declaration = FunctionType::INITIALIZER;
        }

        ResolveFunction(method, declaration);
    }
    EndScope();
    if (stmt.superclass != nullptr)
        EndScope();
    currentClass = enclosingClass;
    return nullptr;
}
Object Resolver::VisitExpressionStmt(Expression &stmt)
{
    Resolve(stmt.expression);
    return nullptr;
}
Object Resolver::VisitFunctionStmt(Function &stmt)
{
    Declare(stmt.name);
    Define(stmt.name);
    ResolveFunction(&stmt, FunctionType::FUNCTION);
    return nullptr;
}
Object Resolver::VisitIfStmt(If &stmt)
{
    Resolve(stmt.condition);
    Resolve(stmt.thenBranch);
    if (stmt.elseBranch != nullptr)
        Resolve(stmt.elseBranch);
    return nullptr;
}
Object Resolver::VisitPrintStmt(Print &stmt)
{
    Resolve(stmt.expression);

    return nullptr;
}
Object Resolver::VisitReturnStmt(Return &stmt)
{
    if (currentFunction == FunctionType::NONE)
    {
        Error::ErrorFind(stmt.keyword, "Can't return from top-level code.");
    }
    if (stmt.value != nullptr)
    {
        if (currentFunction == FunctionType::INITIALIZER)
        {
            Error::ErrorFind(stmt.keyword,
                             "Can't return a value from an initializer.");
        }
        Resolve(stmt.value);
    }

    return nullptr;
}
Object Resolver::VisitVarStmt(Var &stmt)
{
    Declare(stmt.name);
    if (stmt.initializer != nullptr)
    {
        Resolve(stmt.initializer);
    }
    Define(stmt.name);
    return nullptr;
}
Object Resolver::VisitWhileStmt(While &stmt)
{
    Resolve(stmt.condition);
    Resolve(stmt.body);
    return nullptr;
}
Object Resolver::VisitAssignExpr(Assign &expr)
{
    Resolve(expr.value);
    ResolveLocal(&expr, expr.name);
    return nullptr;
}
Object Resolver::VisitBinaryExpr(Binary &expr)
{
    Resolve(expr.left);
    Resolve(expr.right);
    return nullptr;
}
Object Resolver::VisitCallExpr(Call &expr)
{
    Resolve(expr.callee);

    for (Expr *argument : expr.arguments)
    {
        Resolve(argument);
    }

    return nullptr;
}

Object Resolver::VisitGetExpr(Get &expr)
{
    Resolve(expr.object);
    return nullptr;
}
Object Resolver::VisitGroupingExpr(Grouping &expr)
{
    Resolve(expr.expression);
    return nullptr;
}
Object Resolver::VisitLiteralExpr(Literal &expr)
{
    return nullptr;
}
Object Resolver::VisitLogicalExpr(Logical &expr)
{
    Resolve(expr.left);
    Resolve(expr.right);
    return nullptr;
}
Object Resolver::VisitSetExpr(Set &expr)
{
    Resolve(expr.value);
    Resolve(expr.object);
    return nullptr;
}
Object Resolver::VisitSuperExpr(Super &expr)
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
    ResolveLocal(&expr, expr.keyword);
    return nullptr;
}
Object Resolver::VisitThisExpr(This &expr)
{
    if (currentClass == ClassType::NONE_CLASS)
    {
        Error::ErrorFind(expr.keyword, "Can't use 'this' outside of a class.");
        return nullptr;
    }
    ResolveLocal(&expr, expr.keyword);
    return nullptr;
}
Object Resolver::VisitUnaryExpr(Unary &expr)
{
    Resolve(expr.right);
    return nullptr;
}
Object Resolver::VisitVariableExpr(Variable *expr)
{
    if (!scopes.empty() && !scopes.top().empty() && scopes.top().find(expr->name.lexeme) != scopes.top().end() && scopes.top().find(expr->name.lexeme)->second == false)
    {
        Error::ErrorFind(expr->name, "Can't read local variable in its own initializer.");
    }

    ResolveLocal(expr, expr->name);
    return nullptr;
}
void Resolver::Resolve(std::vector<Stmt *> statements)
{
    for (Stmt *statement : statements)
    {
        Resolve(statement);
    }
}
void Resolver::Resolve(Stmt *stmt)
{
    stmt->Accept(*this);
}
void Resolver::Resolve(Expr *expr)
{
    expr->Accept(*this);
}
void Resolver::ResolveFunction(Function *function, FunctionType type)
{

    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;
    // std::stack<std::map<std::string, bool>> temp;
    // temp = scopes;
    // while (!temp.empty())
    // {
    //     std::map<std::string, bool> currentScope = temp.top();
    //     temp.pop();

    //     // 打印当前作用域的内容
    //     for (const auto &entry : currentScope)
    //     {
    //         std::cout << entry.first << ": " << entry.second << std::endl;
    //     }
    //     std::cout << "------" << std::endl;
    // }
    BeginScope(); // 这里进去scopes变为空了
    for (Token param : function->params)
    {
        Declare(param);
        Define(param);
    }

    Resolve(function->body);

    // temp = scopes;
    // while (!temp.empty()) // 这里少一个
    // {
    //     std::map<std::string, bool> currentScope = temp.top();
    //     temp.pop();

    //     // 打印当前作用域的内容
    //     for (const auto &entry : currentScope)
    //     {
    //         std::cout << entry.first << ": " << entry.second << std::endl;
    //     }
    //     std::cout << "------" << std::endl;
    // }
    EndScope();
    currentFunction = enclosingFunction;
}
void Resolver::BeginScope()
{
    // std::stack<std::map<std::string, bool>> temp;
    // temp = scopes;
    // while (!temp.empty()) // 这里少一个
    // {
    //     std::map<std::string, bool> currentScope = temp.top();
    //     temp.pop();

    //     // 打印当前作用域的内容
    //     for (const auto &entry : currentScope)
    //     {
    //         std::cout << entry.first << ": " << entry.second << std::endl;
    //     }
    //     std::cout << "------" << std::endl;
    // }
    std::map<std::string, bool> newScope;
    scopes.push(newScope);

    // temp = scopes;
    // while (!temp.empty()) // 这里少一个
    // {
    //     std::map<std::string, bool> currentScope = temp.top();
    //     temp.pop();

    //     // 打印当前作用域的内容
    //     for (const auto &entry : currentScope)
    //     {
    //         std::cout << entry.first << ": " << entry.second << std::endl;
    //     }
    //     std::cout << "------" << std::endl;
    // }
}
void Resolver::EndScope()
{
    // std::stack<std::map<std::string, bool>> temp;
    // // 复制scopes
    // temp = scopes;
    // while (!temp.empty()) // 这里少一个
    // {
    //     std::map<std::string, bool> currentScope = temp.top();
    //     temp.pop();

    //     // 打印当前作用域的内容
    //     for (const auto &entry : currentScope)
    //     {
    //         std::cout << entry.first << ": " << entry.second << std::endl;
    //     }
    //     std::cout << "------" << std::endl;
    // }

    scopes.pop();
}
void Resolver::Declare(const Token &name)
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
void Resolver::Define(Token &name)
{
    if (scopes.empty())
        return;
    //  std::cout << name.lexeme << std::endl;
    scopes.top()[name.lexeme] = true;
    // std::stack<std::map<std::string, bool>> temp;
    // temp = scopes;
    // while (!temp.empty()) // 这里少一个
    // {
    //     std::map<std::string, bool> currentScope = temp.top();
    //     temp.pop();

    //     // 打印当前作用域的内容
    //     for (const auto &entry : currentScope)
    //     {
    //         std::cout << entry.first << ": " << entry.second << std::endl;
    //     }
    //     std::cout << "------" << std::endl;
    // }
}
void Resolver::ResolveLocal(Expr *expr, const Token &name)
{
    std::stack<std::map<std::string, bool>> temp;
    temp = scopes;
    for (int i = temp.size() - 1; i >= 0; i--)
    {
        const std::map<std::string, bool> &currentScope = scopes.top();
        // 打印scopes
        // for (auto it = currentScope.begin(); it != currentScope.end(); it++)
        // {
        //     std::cout << it->first << " " << it->second << std::endl;
        // }

        auto it = currentScope.find(name.lexeme);
        //  std::cout << name.lexeme << std::endl;
        if (it != currentScope.end())
        {
            interpreter->Resolve(expr, temp.size() - 1 - i);
            scopes = temp; // return之前要恢复scopes
            return;
        }
        scopes.pop();
    }
    scopes = temp; // 恢复scopes

    // std::stack<std::map<std::string, bool>> temp;
    // temp = scopes;
    // while (!scopes.empty())
    // {
    //     std::map<std::string, bool> currentScope = temp.top();
    //     temp.pop();

    //     // 打印当前作用域的内容
    //     for (const auto &entry : currentScope)
    //     {
    //         std::cout << entry.first << ": " << entry.second << std::endl;
    //     }
    //     std::cout << "------" << std::endl;
    // }
    // std::stack<std::map<std::string, bool>> temp;
    //  复制scopes
    // temp = scopes;

    // for (int i = temp.size() - 1; i >= 0; i--) //
    // {
    //     // 得到第i层的scopes
    //     scopes = temp;

    //     for (int j = 0; j < i; j++)
    //     {
    //         scopes.pop();
    //     }

    //     const std::map<std::string, bool> &currentScope = scopes.top();
    //     // 打印scopes
    //     for (auto it = currentScope.begin(); it != currentScope.end(); it++)
    //     {
    //         std::cout << it->first << " " << it->second << std::endl;
    //     }

    //     auto it = currentScope.find(name.lexeme);
    //     std::cout << name.lexeme << std::endl;
    //     if (it != currentScope.end())
    //     {
    //         interpreter->resolve(expr, temp.size() - 1 - i);
    //         scopes = temp; // 忘了加这一句
    //         return;
    //     }
    // }
    // scopes = temp; // 恢复scopes
}
