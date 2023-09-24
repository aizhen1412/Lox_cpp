#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <string>
#include "expr.h"

struct ToStringVisitor
{
    std::string operator()(const double &value) const;
    std::string operator()(const std::string &value) const;
    std::string operator()(std::nullptr_t) const;
};

class AstPrinter : public Visitor
{
public:
    Object print(Expr &expr);
    Object VisitBinary(Binary &expr) override;
    Object VisitGrouping(Grouping &expr) override;
    Object VisitUnary(Unary &expr) override;
    Object VisitLiteral(Literal &expr) override;
    Object VisitCall(Call &Expr) override;
    Object VisitAssign(Assign &Expr) override;
    Object VisitGet(Get &Expr) override;
    Object VisitLogical(Logical &Expr) override;
    Object VisitSet(Set &Expr) override;
    Object VisitSuper(Super &Expr) override;
    Object VisitThis(This &Expr) override;
    Object VisitVariable(Variable &Expr) override;

private:
    template <typename T, typename... Args>
    std::string parenthesize(const T &name, Args... expr);
};

#endif // AST_PRINTER_H