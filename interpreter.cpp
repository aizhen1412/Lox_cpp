#include <typeinfo>
#include "interpreter.h"
#include "error.h"
#include "parser.h"
#include "visit_call_expr.h"
#include "lox_function.h"
#include <variant>
#include "return_method.h"

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
Object Interpreter::VisitVariable(Variable &expr)
{
    return environment->get(expr.name); // retbool
}
Object Interpreter::VisitGrouping(Grouping &expr)
{
    return Evaluate(expr.expression);
}

Object Interpreter::VisitBinary(Binary &expr)
{
    Object left = Evaluate(expr.left);
    Object right = Evaluate(expr.right);

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
    Object callee = Evaluate(expr.callee);

    std::vector<Object> arguments_;
    for (Expr *argument : expr.arguments)
    {
        arguments_.push_back(Evaluate(argument));
    }

    if (!(std::holds_alternative<LoxCallable *>(callee)))
    {
        throw new RuntimeError(expr.paren, "Can only call functions and classes.");
    }
    LoxCallable *function = std::get<LoxCallable *>(callee);
    if (arguments_.size() != function->arity())
    {
        throw new RuntimeError(expr.paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments_.size()) + ".");
    }
    LoxFunction *loxFunction = dynamic_cast<LoxFunction *>(function);
    return loxFunction->call(this, arguments_);
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
void Interpreter::executeBlock(std::vector<Stmt *> statements, Environment *environment)
{
    Environment *previous = this->environment;
    // try
    // {
    this->environment = environment; // 切换环境
    for (auto statement : statements)

    {
        execute(statement);
    }
    this->environment = previous;
    // }
}
Object Interpreter::visitBlockStmt(Block &stmt)
{
    executeBlock(stmt.statements, new Environment(environment));

    return nullptr;
}
Object Interpreter::visitExpressionStmt(Expression &stmt)
{
    Evaluate(stmt.expression);
    return nullptr;
}
Object Interpreter::visitFunctionStmt(Function &stmt)
{
    LoxFunction *function = new LoxFunction(stmt, environment);
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
    environment->assign(expr.name, value);
    return value;
}
void Interpreter::Interpret(std::vector<Stmt *> statements)
{
    // Object value = Evaluate(expression);
    // std::cout << Stringify(value) << std::endl;
    // try
    // {
    //     Object value = Evaluate(expression);
    //     std::cout << Stringify(value) << std::endl;
    // }
    // catch (const RuntimeError &error)
    // {
    //     Error::RuntimeError(error);
    // }
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

    if (std::holds_alternative<double>(object))
    {
        std::string text = std::to_string(std::get<double>(object));
        // Remove ".0" from integer-valued Doubles.
        if (text.length() >= 2 && text.substr(text.length() - 2) == ".0")
        {
            text = text.substr(0, text.length() - 2);
        }

        return text;
    }
    if (std::holds_alternative<bool>(object))
    {
        return std::get<bool>(object) ? "true" : "false";
    }
    if (std::holds_alternative<LoxCallable *>(object))
    {
        return "function";
    }
    return std::get<std::string>(object);
}
