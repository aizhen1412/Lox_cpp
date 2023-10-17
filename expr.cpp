#include "expr.h"

Assign::Assign(Token name, Expr *value) : name(name), value(value) {}
Object Assign::Accept(Visitor &visitor)
{
    return visitor.VisitAssignExpr(*this);
}
Binary::Binary(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
Object Binary::Accept(Visitor &visitor)
{
    return visitor.VisitBinary(*this);
}
Call::Call(Expr *callee, Token paren, std::vector<Expr *> arguments) : callee(callee), paren(paren), arguments(arguments) {}
Object Call::Accept(Visitor &visitor)
{
    return visitor.VisitCall(*this);
}
Get::Get(Expr *object, Token name) : object(object), name(name) {}
Object Get::Accept(Visitor &visitor)
{
    return visitor.VisitGet(*this);
}
Grouping::Grouping(Expr *expression) : expression(expression) {}
Object Grouping::Accept(Visitor &visitor)
{
    return visitor.VisitGrouping(*this);
}
Literal::Literal(Object value) : value(value) {}
Object Literal::Accept(Visitor &visitor)
{
    return visitor.VisitLiteral(*this);
}
Logical::Logical(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
Object Logical::Accept(Visitor &visitor)
{
    return visitor.VisitLogical(*this);
}
Set::Set(Expr *object, Token name, Expr *value) : object(object), name(name), value(value) {}
Object Set::Accept(Visitor &visitor)
{
    return visitor.VisitSet(*this);
}
Super::Super(Token keyword, Token method) : keyword(keyword), method(method) {}
Object Super::Accept(Visitor &visitor)
{
    return visitor.VisitSuper(*this);
}
This::This(Token keyword) : keyword(keyword) {}
Object This::Accept(Visitor &visitor)
{
    return visitor.VisitThis(*this);
}
Unary::Unary(Token op, Expr *right) : op(op), right(right) {}
Object Unary::Accept(Visitor &visitor)
{
    return visitor.VisitUnary(*this);
}
Variable::Variable(Token name) : name(name) {}
Object Variable::Accept(Visitor &visitor)
{
    return visitor.VisitVariable(*this);
}