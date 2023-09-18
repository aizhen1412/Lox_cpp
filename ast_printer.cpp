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
        return "nullptr_t";
    }
};
class AstPrinter : public Visitor
{
public:
    std::string print(Expr &expr)
    {
        return expr.Accept(*this);
    }
    std::string VisitBinary(Binary &expr) override
    {
        return parenthesize(expr.op.lexeme, expr.left, expr.right);
    }
    std::string VisitGrouping(Grouping &expr) override
    {
        return parenthesize("group", expr.expression);
    }

    std::string VisitUnary(Unary &expr) override
    {
        return parenthesize(expr.op.lexeme, expr.right);
    }
    std::string VisitLiteral(Literal &expr) override
    {
        std::string result = std::visit(ToStringVisitor(), expr.value);
        return result;
    }
    std::string VisitCall(Call &Expr) override { return "test"; };
    std::string VisitAssign(Assign &Expr) override { return "test"; };
    std::string VisitGet(Get &Expr) override { return "test"; };
    std::string VisitLogical(Logical &Expr) override { return "test"; };
    std::string VisitSet(Set &Expr) override { return "test"; };
    std::string VisitSuper(Super &Expr) override { return "test"; };
    std::string VisitThis(This &Expr) override { return "test"; };
    std::string VisitVariable(Variable &Expr) override { return "test"; };

private:
    template <typename T, typename... Args>
    std::string parenthesize(const T &name, Args... expr)
    {
        std::stringstream ss;
        ss << "(" << name;

        ((ss << " " << expr->Accept(*this)), ...);

        ss << ")";

        return ss.str();
    }
};

int main()
{
    Binary expression = Binary(
        new Unary(
            Token(TokenType::MINUS, "-", nullptr, 1),
            new Literal(123.0)),
        Token(TokenType::STAR, "*", nullptr, 1),
        new Grouping(
            new Literal(45.67)));

    AstPrinter printer;
    std::cout << printer.print(expression) << std::endl;
    return 0;
}
