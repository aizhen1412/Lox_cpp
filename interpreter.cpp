/*
 * interpreter.cpp
 * This file implements the Interpreter class defined in interpreter.h.
 * The Interpreter class is the core of the Lox language. It interprets and executes Lox code.
 *
 * The constructor initializes the interpreter with a new global environment and a new locals map.
 *
 * The destructor deletes the environments and the locals map.
 *
 * The Interpret method is the entry point of the interpreter. It takes a list of statements and interprets them. If an error occurs during interpretation, it is caught and reported.
 *
 * The ExecuteBlock method executes a block of statements in a given environment. It creates a new environment for the block, executes the statements in this environment, and then restores the previous environment.
 *
 * The Resolve method resolves a variable and stores its depth in the locals map.
 *
 * The Visit... methods are used to visit different types of expressions and statements. They evaluate expressions, execute statements, and handle control flow.
 *
 * The CheckNumberOperand and CheckNumberOperands methods check if the operand(s) of an operation are numbers. If not, they throw a RuntimeError.
 *
 * The IsTruthy method checks if an object is truthy according to the rules of the Lox language.
 *
 * The IsEqual method checks if two objects are equal.
 *
 * The Evaluate method evaluates an expression and returns its value.
 *
 * The Execute method executes a statement. If a return statement is encountered, it throws a Return exception.
 *
 * The LookUpVariable method looks up a variable in the environment. If the variable is not found, it throws a RuntimeError.
 *
 * The Stringify method converts an object to a string.
 */
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

Interpreter::~Interpreter()
{
    // 如果globals和environment指向不同 那么删除
    if (globals != environment)
        delete environment;

    delete globals;
}
void Interpreter::Interpret(std::vector<Stmt *> statements)
{
    try
    {
        for (const auto &statement : statements)
            Execute(statement);
    }
    catch (const RuntimeError &error)
    {
        Error::ProcessRuntimeError(error);
    }
}
void Interpreter::ExecuteBlock(std::vector<Stmt *> statements, Environment *environment)
{
    Environment *previous = this->environment;
    try
    {
        this->environment = environment; // 切换环境
        for (auto statement : statements)
        {
            Execute(statement);
        }
    }
    catch (const Return_method &returnValue)
    {
        this->environment = previous;
        throw returnValue;
        return;
    }
    this->environment = previous; // 抛出return后这里不会执行
}
void Interpreter::Resolve(Expr *expr, int depth)
{
    locals[expr] = depth;
}

Object Interpreter::VisitSuperExpr(Super &Expr)
{
    int distance = locals[&Expr];
    LoxClass *superclass = std::get<LoxClass *>(environment->GetAt(distance, "super"));
    LoxInstance *object = std::get<LoxInstance *>(environment->GetAt(distance - 1, "this"));
    LoxFunction *method = superclass->FindMethod(Expr.method.lexeme);
    if (method == nullptr)
    {
        throw RuntimeError(Expr.method,
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
    throw RuntimeError(expr.name,
                       "Only instances have properties.");
}
Object Interpreter::VisitSetExpr(Set &expr)
{

    Object object = Evaluate(expr.object); // right
    if (!(std::holds_alternative<LoxInstance *>(object)))
    {
        throw RuntimeError(expr.name,
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
    default:
        break;
    }

    // Unreachable.
    return nullptr;
}
Object Interpreter::VisitVariableExpr(Variable *expr)
{
    return LookUpVariable(expr->name, expr);
}

Object Interpreter::VisitGroupingExpr(Grouping &expr)
{
    return Evaluate(expr.expression);
}

Object Interpreter::VisitBinaryExpr(Binary &expr)
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
        throw RuntimeError(expr.op, "Operands must be two numbers or two strings.");
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
    default:
        break;
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
            throw RuntimeError(expr.paren, "Can only call functions and classes.");
        }
        LoxCallable *function = std::get<LoxCallable *>(callee);
        if (static_cast<int>(arguments_.size()) != function->Arity())
        {
            throw RuntimeError(expr.paren, "Expected " + std::to_string(function->Arity()) + " arguments but got " + std::to_string(arguments_.size()) + ".");
        }
        Object ret = function->Call(this, arguments_);
        // delete function;
        return ret;
    }
    else
    {
        if (!(std::holds_alternative<LoxCallable *>(callee)))
        {
            throw RuntimeError(expr.paren, "Can only call functions and classes.");
        }
        LoxCallable *function = std::get<LoxCallable *>(callee);
        if (static_cast<int>(arguments_.size()) != function->Arity()) // Cast the size of arguments_ to int
        {
            throw RuntimeError(expr.paren, "Expected " + std::to_string(function->Arity()) + " arguments but got " + std::to_string(arguments_.size()) + ".");
        }
        LoxFunction *loxFunction = dynamic_cast<LoxFunction *>(function);
        Object ret = loxFunction->Call(this, arguments_);
        // delete function;
        return ret;
    }
}
void Interpreter::CheckNumberOperand(Token op, Object operand) // 检查操作数是否为数字
{
    if (std::holds_alternative<double>(operand))
        return;
    throw RuntimeError(op, "Operand must be a number.");
}
void Interpreter::CheckNumberOperands(Token op, Object left, Object right) // 检查操作数是否为数字
{
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
        return;

    throw RuntimeError(op, "Operands must be numbers.");
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
Object Interpreter::LookUpVariable(Token name, Expr *expr)
{
    auto ret = locals.find(expr);
    if (ret != locals.end())
    {
        int distance = ret->second;
        return environment->GetAt(distance, name.lexeme); //
    }
    else
    {
        return globals->Get(name);
    }
}
Object Interpreter::VisitBlockStmt(Block &stmt)
{
    Environment *new_environment = new Environment(environment);

    ExecuteBlock(stmt.statements, new_environment);

    delete new_environment;

    return nullptr;
}
Object Interpreter::VisitClassStmt(Class &stmt)
{
    Object superclass = nullptr;
    if (stmt.superclass != nullptr) // 存在父类
    {
        superclass = Evaluate(stmt.superclass);

        if (!(std::holds_alternative<LoxClass *>(superclass)))
            throw RuntimeError(stmt.superclass->name, "Superclass must be a class.");
    }
    environment->Define(stmt.name.lexeme, nullptr);
    if (stmt.superclass != nullptr)
    {
        environment = new Environment(environment); // 原来的环境保存在environment->enclosing中
        environment->Define("super", superclass);   // 现在的environment是interpreter中的environment，会在interpreter的析构函数中被delete
    }
    std::unordered_map<std::string, LoxFunction *> methods;
    for (Function *method : stmt.methods)
    {
        LoxFunction *function = new LoxFunction(*method, environment, (method->name.lexeme.compare("init") == 0)); // function在LoxClass的析构函数中会被delete
        methods[method->name.lexeme] = function;
    }

    LoxClass *klass = nullptr;
    if (std::holds_alternative<std::nullptr_t>(superclass)) // superclass == null
    {                                                       // klass在environment的析构函数中会被delete
        klass = new LoxClass(stmt.name.lexeme, std::get<std::nullptr_t>(superclass), methods);
    }
    else
    {
        klass = new LoxClass(stmt.name.lexeme, std::get<LoxClass *>(superclass), methods);
        environment = environment->Get_enclosing(); // 退出环境
    }

    environment->Assign(stmt.name, klass); // 将类名和类的映射关系存入环境中

    return nullptr;
}
Object Interpreter::VisitExpressionStmt(Expression &stmt)
{
    Evaluate(stmt.expression);
    return nullptr;
}
Object Interpreter::VisitFunctionStmt(Function &stmt)
{
    LoxFunction *function = new LoxFunction(stmt, environment, false); // function在environment的析构函数中会被delete

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
        Execute(stmt.body);

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

std::string Interpreter::Stringify(Object object)
{
    if (std::holds_alternative<std::nullptr_t>(object))
        return "nil";

    else if (std::holds_alternative<double>(object))
    {
        std::string text = std::to_string(std::get<double>(object));

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
