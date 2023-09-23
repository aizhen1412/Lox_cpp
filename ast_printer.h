#ifndef AST_PRINTER_H
#define AST_PRINTER_H
#include <string>
#include <sstream>
#include "expr.h"

struct ToStringVisitor
{
    std::string operator()(const double &value) const
    {
        return std::to_string(value);
    }

    std::string operator()(const std::string &value) const
    {
        return value;
    }

    std::string operator()(std::nullptr_t) const
    {
        return "nil";
    }
};
class AstPrinter : public Visitor
{
public:
    Object print(Expr &expr)
    {
        return expr.Accept(*this);
    }
    Object VisitBinary(Binary &expr) override
    {
        return parenthesize(expr.op.lexeme, expr.left, expr.right);
    }
    Object VisitGrouping(Grouping &expr) override
    {
        return parenthesize("group", expr.expression);
    }

    Object VisitUnary(Unary &expr) override
    {
        return parenthesize(expr.op.lexeme, expr.right);
    }
    Object VisitLiteral(Literal &expr) override
    {
        std::string result = std::visit(ToStringVisitor(), expr.value);
        return result;
    }
    Object VisitCall(Call &Expr) override { return "test"; };
    Object VisitAssign(Assign &Expr) override { return "test"; };
    Object VisitGet(Get &Expr) override { return "test"; };
    Object VisitLogical(Logical &Expr) override { return "test"; };
    Object VisitSet(Set &Expr) override { return "test"; };
    Object VisitSuper(Super &Expr) override { return "test"; };
    Object VisitThis(This &Expr) override { return "test"; };
    Object VisitVariable(Variable &Expr) override { return "test"; };

private:
    template <typename T, typename... Args>
    std::string parenthesize(const T &name, Args... expr)
    {
        std::stringstream ss;
        ss << "(" << name;

        ((ss << " " << std::get<std::string>(expr->Accept(*this))), ...);

        ss << ")";

        return ss.str();
    }
};
#endif // AST_PRINTER_H
