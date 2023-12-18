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

Object Interpreter::VisitSuper(Super &Expr)
{
    int distance = locals[&Expr];
    LoxClass *superclass = std::get<LoxClass *>(environment->getAt(distance, "super"));
    LoxInstance *object = std::get<LoxInstance *>(environment->getAt(distance - 1, "this"));
    LoxFunction *method = superclass->findMethod(Expr.method.lexeme);
    if (method == nullptr)
    {
        throw new RuntimeError(Expr.method,
                               "Undefined property '" + Expr.method.lexeme + "'.");
    }

    return method->bind(object);
}
Object Interpreter::VisitLiteral(Literal &expr)
{
    return expr.value;
}
Object Interpreter::VisitLogical(Logical &expr)
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
Object Interpreter::VisitGet(Get &expr)
{
    Object object = Evaluate(expr.object);
    if (std::holds_alternative<LoxInstance *>(object))
    {
        return ((std::get<LoxInstance *>(object))->get(expr.name));
    }
    if (std::holds_alternative<std::nullptr_t>(object))
    {
        std::cout << "err" << std::endl;
    }
    throw new RuntimeError(expr.name,
                           "Only instances have properties.");
}
Object Interpreter::VisitSet(Set &expr)
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
    (std::get<LoxInstance *>(object))->set(expr.name, value);
    return value;
}
Object Interpreter::VisitThis(This &expr)
{
    return lookUpVariable(expr.keyword, &expr);
}
Object Interpreter::VisitUnary(Unary &expr)
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
Object Interpreter::VisitVariable(Variable *expr)
{
    // return environment->get(expr.name); // retbool
    return lookUpVariable(expr->name, expr);
}
Object Interpreter::lookUpVariable(Token name, Expr *expr)
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
        return environment->getAt(distance, name.lexeme); //
    }
    else
    {
        // std::cout << "globals" << std::endl;
        return globals->get(name);
    }
}
Object Interpreter::VisitGrouping(Grouping &expr)
{
    return Evaluate(expr.expression);
}

Object Interpreter::VisitBinary(Binary &expr)
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
Object Interpreter::VisitCall(Call &expr)
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
        if (arguments_.size() != function->arity())
        {
            throw new RuntimeError(expr.paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments_.size()) + ".");
        }

        return function->call(this, arguments_);
    }
    else
    {
        if (!(std::holds_alternative<LoxCallable *>(callee)))
        {
            throw new RuntimeError(expr.paren, "Can only call functions and classes.");
        }
        LoxCallable *function = std::get<LoxCallable *>(callee);
        if (arguments_.size() != function->arity()) // arity() 返回函数的参数个数
        {
            throw new RuntimeError(expr.paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments_.size()) + ".");
        }
        LoxFunction *loxFunction = dynamic_cast<LoxFunction *>(function);
        return loxFunction->call(this, arguments_);
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
void Interpreter::execute(Stmt *stmt)
{
    stmt->accept(*this);
}
void Interpreter::resolve(Expr *expr, int depth)
{
    locals[expr] = depth;
}
void Interpreter::executeBlock(std::vector<Stmt *> statements, Environment *environment)
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
            execute(statement);
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
Object Interpreter::visitBlockStmt(Block &stmt)
{
    executeBlock(stmt.statements, new Environment(environment));

    return nullptr;
}
Object Interpreter::visitClassStmt(Class &stmt)
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
    environment->define(stmt.name.lexeme, nullptr);
    if (stmt.superclass != nullptr)
    {
        environment = new Environment(environment);
        environment->define("super", superclass);
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
    environment->assign(stmt.name, klass);
    return nullptr;
}
Object Interpreter::visitExpressionStmt(Expression &stmt)
{
    Evaluate(stmt.expression);
    return nullptr;
}
Object Interpreter::visitFunctionStmt(Function &stmt)
{
    LoxFunction *function = new LoxFunction(stmt, environment, false);

    LoxCallable *callable = function;
    environment->define(stmt.name.lexeme, callable);
    return nullptr;
}
Object Interpreter::visitIfStmt(If &stmt)
{
    if (IsTruthy(Evaluate(stmt.condition)))
    {
        execute(stmt.thenBranch);
    }
    else if (stmt.elseBranch != nullptr)
    {
        execute(stmt.elseBranch);
    }
    return nullptr;
}
Object Interpreter::visitPrintStmt(Print &stmt)
{
    Object value = Evaluate(stmt.expression);
    // 第三次
    std::cout << Stringify(value) << std::endl;
    return nullptr;
}
Object Interpreter::visitReturnStmt(Return &stmt)
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
Object Interpreter::visitVarStmt(Var &stmt)
{
    Object value = nullptr;
    if (stmt.initializer != nullptr)
    {
        value = Evaluate(stmt.initializer);
    }

    environment->define(stmt.name.lexeme, value);
    return nullptr;
}
Object Interpreter::visitWhileStmt(While &stmt)
{
    while (IsTruthy(Evaluate(stmt.condition)))
    {
        execute(stmt.body);
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
        environment->assignAt(distance, expr.name, value);
    }
    else
    {
        globals->assign(expr.name, value);
    }

    return value;
}
void Interpreter::Interpret(std::vector<Stmt *> statements)
{
    try
    {
        for (const auto &statement : statements)
        {
            execute(statement);
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
        return klass->toString();
    }
    else if (std::holds_alternative<LoxInstance *>(object))
    {
        LoxInstance *klass = std::get<LoxInstance *>(object);
        return klass->toString();
    }

    else
        return std::get<std::string>(object);
}
