#include <typeinfo>
#include "interpreter.h"
#include "error.h"
#include "parser.h"
#include "visit_call_expr.h"
#include "lox_function.h"
#include <variant>
#include "return_method.h"
#include "lox_class.h"
#include "lox_instance.h"

Object Interpreter::VisitSuperExpr(Super &Expr)
{
    int distance = locals[&Expr];
    LoxClass *superclass = std::get<LoxClass *>(environment->GetAt(distance, "super"));
    LoxInstance *object = std::get<LoxInstance *>(environment->GetAt(distance - 1, "this"));
    LoxFunction *method = superclass->FindMethod(Expr.method.lexeme);
    if (method == nullptr)
    {
        throw new RuntimeError(Expr.method,
                               "Undefined property '" + Expr.method.lexeme + "'.");
    }

    return method->Bind(object);
}
Object Interpreter::VisitLiteralExpr(Literal &expr)
{
    return expr.value;
}
Object Interpreter::VisitLogicalExpr(Logical &expr)
{
    Object left = Evaluate(expr.left);

    if (expr.op.type == OR)
    {
        if (IsTruthy(left))
            return left;
    }
    else
    {
        if (!IsTruthy(left))
            return left;
    }

    return Evaluate(expr.right);
}
Object Interpreter::VisitGetExpr(Get &expr)
{
    Object object = Evaluate(expr.object);
    if (std::holds_alternative<LoxInstance *>(object))
    {
        return ((std::get<LoxInstance *>(object))->Get(expr.name));
    }
    if (std::holds_alternative<std::nullptr_t>(object))
    {
        std::cout << "err" << std::endl;
    }
    throw new RuntimeError(expr.name,
                           "Only instances have properties.");
}
Object Interpreter::VisitSetExpr(Set &expr)
{
    //  std::cout << "set" << expr.value << std::endl;

    Object object = Evaluate(expr.object); // right
    //   Object object = Evaluate(expr.value); // test

    // if ((std::holds_alternative<double>(object)))
    // {
    //     std::cout << std::get<double>(object) << std::endl;
    // }
    if (!(std::holds_alternative<LoxInstance *>(object)))
    {
        throw new RuntimeError(expr.name,
                               "Only instances have fields.");
    }

    Object value = Evaluate(expr.value);
    (std::get<LoxInstance *>(object))->Set(expr.name, value);
    return value;
}
Object Interpreter::VisitThisExpr(This &expr)
{
    return LookUpVariable(expr.keyword, &expr);
}
Object Interpreter::VisitUnaryExpr(Unary &expr)
{
    Object right = Evaluate(expr.right);

    switch (expr.op.type)
    {
    case BANG:
        return !IsTruthy(right);
    case MINUS:
        CheckNumberOperand(expr.op, right);
        return -std::get<double>(right);
    }

    // Unreachable.
    return nullptr;
}
Object Interpreter::VisitVariableExpr(Variable *expr)
{
    // return environment->get(expr.name); // retbool
    return LookUpVariable(expr->name, expr);
}
Object Interpreter::LookUpVariable(Token name, Expr *expr)
{
    // 打印locals
    // int i = 0;
    // for (auto it = this->locals.begin(); it != this->locals.end(); it++)
    // {
    //     std::cout << expr << std::endl;
    //     std::cout << "it->first:" << it->first << std::endl;
    //     std::cout << "it->second:" << it->second << std::endl;
    //     std::cout << "        " << std::endl;
    // }
    auto ret = locals.find(expr);
    if (ret != locals.end())
    {
        int distance = ret->second;
        // std::cout << "distance" << distance << std::endl;
        return environment->GetAt(distance, name.lexeme); //
    }
    else
    {
        // std::cout << "globals" << std::endl;
        return globals->get(name);
    }
}
Object Interpreter::VisitGroupingExpr(Grouping &expr)
{
    return Evaluate(expr.expression);
}

Object Interpreter::VisitBinaryExpr(Binary &expr)
{
    Object left = Evaluate(expr.left);
    //  std::cout << "left" << std::get<double>(left) << std::endl;
    Object right = Evaluate(expr.right);
    // std::cout << "right" << std::get<double>(right) << std::endl;

    switch (expr.op.type)
    {
    case GREATER:
        CheckNumberOperands(expr.op, left, right);
        return std::get<double>(left) > std::get<double>(right);
    case GREATER_EQUAL:
        CheckNumberOperands(expr.op, left, right);
        return std::get<double>(left) >= std::get<double>(right);
    case LESS:
        CheckNumberOperands(expr.op, left, right);
        return std::get<double>(left) < std::get<double>(right);
    case LESS_EQUAL:
        CheckNumberOperands(expr.op, left, right);
        return std::get<double>(left) <= std::get<double>(right);
    case MINUS:
        CheckNumberOperands(expr.op, left, right);
        return std::get<double>(left) - std::get<double>(right);
    case PLUS:
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        {
            return std::get<double>(left) + std::get<double>(right);
        }

        if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
        {
            return std::get<std::string>(left) + std::get<std::string>(right);
        }
        throw new RuntimeError(expr.op, "Operands must be two numbers or two strings.");
        break;
    case SLASH:
        CheckNumberOperands(expr.op, left, right);
        return std::get<double>(left) / std::get<double>(right);
    case STAR:
        CheckNumberOperands(expr.op, left, right);
        return std::get<double>(left) * std::get<double>(right);
    case BANG_EQUAL:
        return !IsEqual(left, right);
    case EQUAL_EQUAL:
        return IsEqual(left, right);
    }

    // Unreachable.
    return nullptr;
}
Object Interpreter::VisitCallExpr(Call &expr)
{
    Object callee = Evaluate(expr.callee); // bool

    std::vector<Object> arguments_;
    for (Expr *argument : expr.arguments)
    {
        arguments_.push_back(Evaluate(argument));
    }
    if ((std::holds_alternative<LoxClass *>(callee)))
    {
        LoxCallable *temp = static_cast<LoxCallable *>(std::get<LoxClass *>(callee));
        callee = temp;
        if (!(std::holds_alternative<LoxCallable *>(callee)))
        {
            throw new RuntimeError(expr.paren, "Can only call functions and classes.");
        }
        LoxCallable *function = std::get<LoxCallable *>(callee);
        if (arguments_.size() != function->Arity())
        {
            throw new RuntimeError(expr.paren, "Expected " + std::to_string(function->Arity()) + " arguments but got " + std::to_string(arguments_.size()) + ".");
        }

        return function->Call(this, arguments_);
    }
    else
    {
        if (!(std::holds_alternative<LoxCallable *>(callee)))
        {
            throw new RuntimeError(expr.paren, "Can only call functions and classes.");
        }
        LoxCallable *function = std::get<LoxCallable *>(callee);
        if (arguments_.size() != function->Arity()) // arity() 返回函数的参数个数
        {
            throw new RuntimeError(expr.paren, "Expected " + std::to_string(function->Arity()) + " arguments but got " + std::to_string(arguments_.size()) + ".");
        }
        LoxFunction *loxFunction = dynamic_cast<LoxFunction *>(function);
        return loxFunction->Call(this, arguments_);
    }
}
void Interpreter::CheckNumberOperand(Token op, Object operand) // 检查操作数是否为数字
{
    if (std::holds_alternative<double>(operand))
        return;
    throw new RuntimeError(op, "Operand must be a number.");
}

void Interpreter::CheckNumberOperands(Token op, Object left, Object right) // 检查操作数是否为数字
{
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        return;

    throw new RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::IsTruthy(Object object)
{
    if (std::holds_alternative<std::nullptr_t>(object))
        return false;
    if (std::holds_alternative<bool>(object))
        return std::get<bool>(object);
    return true;
}

bool Interpreter::IsEqual(Object a, Object b)
{
    if (std::holds_alternative<std::nullptr_t>(a) && std::holds_alternative<std::nullptr_t>(b))
        return true;
    if (std::holds_alternative<std::nullptr_t>(a))
        return false;

    if (a.index() != b.index())
    {
        return false;
    }

    if (std::holds_alternative<double>(a))
    {
        return std::get<double>(a) == std::get<double>(b);
    }
    else if (std::holds_alternative<std::string>(a))
    {
        return std::get<std::string>(a) == std::get<std::string>(b);
    }
    else if (std::holds_alternative<std::nullptr_t>(a))
    {
        return true;
    }
    else if (std::holds_alternative<bool>(a))
    {
        return std::get<bool>(a) == std::get<bool>(b);
    }

    return false;
}

Object Interpreter::Evaluate(Expr *expr)
{
    return expr->Accept(*this);
}
void Interpreter::Execute(Stmt *stmt)
{
    stmt->Accept(*this);
}
void Interpreter::Resolve(Expr *expr, int depth)
{
    locals[expr] = depth;
}
void Interpreter::ExecuteBlock(std::vector<Stmt *> statements, Environment *environment)
{
    // std::cout << "call executeBlock " << std::endl;
    Environment *previous = this->environment;
    // std::cout << "first" << std::get<double>(previous->values["n"]) << std::endl;
    try
    {
        this->environment = environment; // 切换环境
        for (auto statement : statements)
        {
            // std::cout << "environment->values[\" n \"] " << std::get<double>(environment->values["n"]) << std::endl;
            // test();
            Execute(statement);
        }
    }
    catch (const Return_method &returnValue)
    {
        this->environment = previous;
        //  std::cout << "call recover " << std::endl;
        throw returnValue;
        return;
    }
    this->environment = previous; // 抛出return后这里不会执行

    // std::cout << "call recover " << std::endl;
    // std::cout << "after throw  " << std::endl;
    // test();
    // std::cout << "second" << std::get<double>(environment->values["n"]) << std::endl;
}
Object Interpreter::VisitBlockStmt(Block &stmt)
{
    ExecuteBlock(stmt.statements, new Environment(environment));

    return nullptr;
}
Object Interpreter::VisitClassStmt(Class &stmt)
{
    Object superclass = nullptr;
    if (stmt.superclass != nullptr) // here is nullptr
    {
        superclass = Evaluate(stmt.superclass);
        // if ((std::holds_alternative<std::nullptr_t>(superclass)))
        // {
        //     std::cout << "superclass == null" << std::endl;
        // }
        if (!(std::holds_alternative<LoxClass *>(superclass)))
        {
            throw new RuntimeError(stmt.superclass->name, "Superclass must be a class.");
        }
    }
    environment->Define(stmt.name.lexeme, nullptr);
    if (stmt.superclass != nullptr)
    {
        environment = new Environment(environment);
        environment->Define("super", superclass);
    }
    std::unordered_map<std::string, LoxFunction *> methods;
    for (Function *method : stmt.methods)
    {
        LoxFunction *function = new LoxFunction(*method, environment, (method->name.lexeme.compare("init") == 0));
        methods[method->name.lexeme] = function;
    }

    LoxClass *klass = nullptr;
    if ((std::holds_alternative<std::nullptr_t>(superclass)))
        klass = new LoxClass(stmt.name.lexeme, std::get<std::nullptr_t>(superclass), methods);
    else
        klass = new LoxClass(stmt.name.lexeme, std::get<LoxClass *>(superclass), methods);

    if (!(std::holds_alternative<std::nullptr_t>(superclass))) // superclass != null
    {
        environment = environment->enclosing;
    }
    environment->Assign(stmt.name, klass);
    return nullptr;
}
Object Interpreter::VisitExpressionStmt(Expression &stmt)
{
    Evaluate(stmt.expression);
    return nullptr;
}
Object Interpreter::VisitFunctionStmt(Function &stmt)
{
    LoxFunction *function = new LoxFunction(stmt, environment, false);

    LoxCallable *callable = function;
    environment->Define(stmt.name.lexeme, callable);
    return nullptr;
}
Object Interpreter::VisitIfStmt(If &stmt)
{
    if (IsTruthy(Evaluate(stmt.condition)))
    {
        Execute(stmt.thenBranch);
    }
    else if (stmt.elseBranch != nullptr)
    {
        Execute(stmt.elseBranch);
    }
    return nullptr;
}
Object Interpreter::VisitPrintStmt(Print &stmt)
{
    Object value = Evaluate(stmt.expression);
    // 第三次
    std::cout << Stringify(value) << std::endl;
    return nullptr;
}
Object Interpreter::VisitReturnStmt(Return &stmt)
{
    Object value = nullptr;
    if (stmt.value != nullptr)
    {
        value = Evaluate(stmt.value);
    }
    //  std::cout << "call return " << std::endl;
    throw Return_method(value);

    return nullptr;
}
Object Interpreter::VisitVarStmt(Var &stmt)
{
    Object value = nullptr;
    if (stmt.initializer != nullptr)
    {
        value = Evaluate(stmt.initializer);
    }

    environment->Define(stmt.name.lexeme, value);
    return nullptr;
}
Object Interpreter::VisitWhileStmt(While &stmt)
{
    while (IsTruthy(Evaluate(stmt.condition)))
    {
        Execute(stmt.body);
    }
    return nullptr;
}
Object Interpreter::VisitAssignExpr(Assign &expr)
{
    Object value = Evaluate(expr.value);
    auto ret = locals.find(&expr);
    if (ret != locals.end())
    {
        int distance = ret->second;
        environment->AssignAt(distance, expr.name, value);
    }
    else
    {
        globals->Assign(expr.name, value);
    }

    return value;
}
void Interpreter::Interpret(std::vector<Stmt *> statements)
{
    try
    {
        for (const auto &statement : statements)
        {
            Execute(statement);
        }
    }
    catch (const RuntimeError &error)
    {
        Error::RuntimeError(error);
    }
}

std::string Interpreter::Stringify(Object object)
{
    if (std::holds_alternative<std::nullptr_t>(object))
        return "nil";

    else if (std::holds_alternative<double>(object))
    {
        std::string text = std::to_string(std::get<double>(object));
        // Remove ".0" from integer-valued Doubles.
        // if (text.length() >= 2 && text.substr(text.length() - 2) == ".0")
        // {
        //     text = text.substr(0, text.length() - 2);
        // }

        size_t dotPosition = text.find('.');
        if (dotPosition != std::string::npos)
        {
            size_t lastNonZero = text.find_last_not_of('0');
            if (lastNonZero == dotPosition)
            {
                // The decimal point is the last non-zero character
                return text.substr(0, dotPosition);
            }
            else
            {
                // Remove trailing zeros after the decimal point
                return text.substr(0, lastNonZero + 1);
            }
        }
        return text;
    }
    else if (std::holds_alternative<bool>(object))
    {
        return std::get<bool>(object) ? "true" : "false";
    }
    else if (std::holds_alternative<LoxCallable *>(object))
    {
        return "function";
    }
    else if (std::holds_alternative<LoxClass *>(object))
    {
        LoxClass *klass = std::get<LoxClass *>(object);
        return klass->ToString();
    }
    else if (std::holds_alternative<LoxInstance *>(object))
    {
        LoxInstance *klass = std::get<LoxInstance *>(object);
        return klass->ToString();
    }

    else
        return std::get<std::string>(object);
}
