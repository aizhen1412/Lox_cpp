#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "runtime_error.h"
#include "error.h"
#include <any>
#include <iostream>
#include <sstream>
#include "ast_printer.h"

class Interpreter : AstPrinter // 后面换成visitor
{
public:
    Object VisitLiteral(Literal &expr)
    {
        return expr.value;
    }
    Object VisitUnary(Unary &expr)
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
    Object VisitGrouping(Grouping &expr)
    {
        return Evaluate(expr.expression);
    }
    Object VisitBinary(Binary &expr)
    {
        Object left = Evaluate(expr.left);
        Object right = Evaluate(expr.right);

        switch (expr.op.type)
        {
        case GREATER:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) > std::get<double>(right);
        case GREATER_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) >= std::get<double>(right);
        case LESS:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) < std::get<double>(right);
        case LESS_EQUAL:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) <= std::get<double>(right);
        case MINUS:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) - std::get<double>(right);
        case PLUS:
            if (std::holds_alternative<double>(left) && std::holds_alternative<double>(left))
            {
                return std::get<double>(left) + std::get<double>(right);
            }

            if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(left))
            {
                return std::get<std::string>(left) + std::get<std::string>(right);
            }
            throw new RuntimeError(expr.op, "Operands must be two numbers or two strings.");
            break;
        case SLASH:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) / std::get<double>(right);
        case STAR:
            checkNumberOperands(expr.op, left, right);
            return std::get<double>(left) * std::get<double>(right);
        case BANG_EQUAL:
            return !IsEqual(left, right);
        case EQUAL_EQUAL:
            return IsEqual(left, right);
        }

        // Unreachable.
        return nullptr;
    }
    void CheckNumberOperand(Token op, Object operand)
    {
        if (std::holds_alternative<double>(operand))
            return;
        throw new RuntimeError(op, "Operand must be a number.");
    }
    void checkNumberOperands(Token op, Object left, Object right)
    {
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
            return;

        throw new RuntimeError(op, "Operands must be numbers.");
    }

    bool IsTruthy(Object object)
    {
        if (std::holds_alternative<std::nullptr_t>(object))
            return false;
        if (std::holds_alternative<bool>(object))
            return std::get<bool>(object);
        return true;
    }
    bool IsEqual(Object a, Object b)
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

    Object Evaluate(Expr *expr)
    {
        return expr->Accept(*this);
    }
    void Interpret(Expr *expression)
    {
        Object value = Evaluate(expression);
        std::cout << Stringify(value) << std::endl;
        // try
        // {
        //     Object value = evaluate(expression);
        //     std::cout << stringify(value) << std::endl;
        // }
        // catch (RuntimeError error)
        // {
        //     Error::runtimeError(error);
        // }
    }
    std::string Stringify(Object object)
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
        return std::get<std::string>(object);
    }
};
#endif // INTERPRETER_H