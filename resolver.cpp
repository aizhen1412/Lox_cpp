#include "resolver.h"
#include "error.h"
#include "lox_instance.h"
#include "interpreter.h"

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
Object Resolver::visitClassStmt(Class &stmt)
{
    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;
    declare(stmt.name);
    define(stmt.name);
    if (stmt.superclass != nullptr && stmt.name.lexeme == stmt.superclass->name.lexeme)
    {
        Error::ErrorFind(stmt.superclass->name, "A class can't inherit from itself.");
    }
    if (stmt.superclass != nullptr)
    {
        currentClass = ClassType::SUBCLASS;
        resolve(stmt.superclass);
    }
    if (stmt.superclass != nullptr)
    {
        beginScope();
        scopes.top()["super"] = true;
    }
    beginScope();
    scopes.top()["this"] = true;
    for (Function *method : stmt.methods)
    {
        FunctionType declaration = FunctionType::METHOD;
        if (method->name.lexeme.compare("init") == 0)
        {
            declaration = FunctionType::INITIALIZER;
        }

        resolveFunction(method, declaration);
    }
    endScope();
    if (stmt.superclass != nullptr)
        endScope();
    currentClass = enclosingClass;
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
        if (currentFunction == FunctionType::INITIALIZER)
        {
            Error::ErrorFind(stmt.keyword,
                             "Can't return a value from an initializer.");
        }
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

Object Resolver::VisitGet(Get &expr)
{
    resolve(expr.object);
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
Object Resolver::VisitSet(Set &expr)
{
    resolve(expr.value);
    resolve(expr.object);
    return nullptr;
}
Object Resolver::VisitThis(This &expr)
{
    if (currentClass == ClassType::NONE_CLASS)
    {
        Error::ErrorFind(expr.keyword, "Can't use 'this' outside of a class.");
        return nullptr;
    }
    resolveLocal(&expr, expr.keyword);
    return nullptr;
}
Object Resolver::VisitUnary(Unary &expr)
{
    resolve(expr.right);
    return nullptr;
}
Object Resolver::VisitVariable(Variable *expr)
{
    if (!scopes.empty() && !scopes.top().empty() && scopes.top().find(expr->name.lexeme) != scopes.top().end() && scopes.top().find(expr->name.lexeme)->second == false)
    {
        Error::ErrorFind(expr->name, "Can't read local variable in its own initializer.");
    }

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
    beginScope(); // 这里进去scopes变为空了
    for (Token param : function->params)
    {
        declare(param);
        define(param);
    }

    resolve(function->body);

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
    endScope();
    currentFunction = enclosingFunction;
}
void Resolver::beginScope()
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
void Resolver::endScope()
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
void Resolver::resolveLocal(Expr *expr, const Token &name)
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
            interpreter->resolve(expr, temp.size() - 1 - i);
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
