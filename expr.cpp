#include "expr.h"
#include <iostream>
#include <typeinfo>
#include "interpreter.h" //
#include "resolver.h"

Assign::Assign(Token name, Expr *value) : name(name), value(value) {}
Object Assign::Accept(Visitor &visitor)
{
    return visitor.VisitAssignExpr(*this);
}
Binary::Binary(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
Object Binary::Accept(Visitor &visitor)
{
    return visitor.VisitBinaryExpr(*this);
}
Call::Call(Expr *callee, Token paren, std::vector<Expr *> arguments) : callee(callee), paren(paren), arguments(arguments) {}
Object Call::Accept(Visitor &visitor)
{
    return visitor.VisitCallExpr(*this);
}
Get::Get(Expr *object, Token name) : object(object), name(name) {}
Object Get::Accept(Visitor &visitor)
{
    return visitor.VisitGetExpr(*this);
}
Grouping::Grouping(Expr *expression) : expression(expression) {}
Object Grouping::Accept(Visitor &visitor)
{
    return visitor.VisitGroupingExpr(*this);
}
Literal::Literal(Object value) : value(value) {}
Object Literal::Accept(Visitor &visitor)
{
    return visitor.VisitLiteralExpr(*this);
}
Logical::Logical(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
Object Logical::Accept(Visitor &visitor)
{
    return visitor.VisitLogicalExpr(*this);
}
Set::Set(Expr *object, Token name, Expr *value) : object(object), name(name), value(value) {}
Object Set::Accept(Visitor &visitor)
{
    return visitor.VisitSetExpr(*this);
}
Super::Super(Token keyword, Token method) : keyword(keyword), method(method) {}
Object Super::Accept(Visitor &visitor)
{
    return visitor.VisitSuperExpr(*this);
}
This::This(Token keyword) : keyword(keyword) {}
Object This::Accept(Visitor &visitor)
{
    return visitor.VisitThisExpr(*this);
}
Unary::Unary(Token op, Expr *right) : op(op), right(right) {}
Object Unary::Accept(Visitor &visitor)
{
    return visitor.VisitUnaryExpr(*this);
}
Variable::Variable(Token name) : name(name) {}
Object Variable::Accept(Visitor &visitor)
{
    return visitor.VisitVariableExpr(this);
}