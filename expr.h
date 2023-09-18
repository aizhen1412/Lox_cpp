#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include "token.h"

#define Object std::variant<double, std::string, std::nullptr_t>

class Visitor;
class Assign;
class Binary;
class Call;
class Get;
class Grouping;
class Literal;
class Logical;
class Set;
class Super;
class This;
class Unary;
class Variable;

class Expr
{
public:
  virtual ~Expr() {}
  virtual std::string Accept(Visitor &visitor) = 0;
};

class Visitor
{
public:
  virtual std::string VisitAssign(Assign &Expr) = 0;
  virtual std::string VisitBinary(Binary &Expr) = 0;
  virtual std::string VisitCall(Call &Expr) = 0;
  virtual std::string VisitGet(Get &Expr) = 0;
  virtual std::string VisitGrouping(Grouping &Expr) = 0;
  virtual std::string VisitLiteral(Literal &Expr) = 0;
  virtual std::string VisitLogical(Logical &Expr) = 0;
  virtual std::string VisitSet(Set &Expr) = 0;
  virtual std::string VisitSuper(Super &Expr) = 0;
  virtual std::string VisitThis(This &Expr) = 0;
  virtual std::string VisitUnary(Unary &Expr) = 0;
  virtual std::string VisitVariable(Variable &Expr) = 0;
};

class Assign : public Expr
{
public:
  Assign(Token name, Expr *value) : name(name), value(value) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitAssign(*this);
  }
  Token name;
  Expr *value;
};

class Binary : public Expr
{
public:
  Binary(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitBinary(*this);
  }
  Expr *left;
  Token op;
  Expr *right;
};

class Call : public Expr
{
public:
  Call(Expr *callee, Token paren, std::vector<Expr *> arguments) : callee(callee), paren(paren), arguments(arguments) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitCall(*this);
  }
  Expr *callee;
  Token paren;
  std::vector<Expr *> arguments;
};

class Get : public Expr
{
public:
  Get(Expr *object, Token name) : object(object), name(name) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitGet(*this);
  }
  Expr *object;
  Token name;
};

class Grouping : public Expr
{
public:
  Grouping(Expr *expression) : expression(expression) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitGrouping(*this);
  }
  Expr *expression;
};

class Literal : public Expr
{
public:
  Literal(Object value) : value(value) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitLiteral(*this);
  }
  Object value;
};

class Logical : public Expr
{
public:
  Logical(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitLogical(*this);
  }
  Expr *left;
  Token op;
  Expr *right;
};

class Set : public Expr
{
public:
  Set(Expr *object, Token name, Expr *value) : object(object), name(name), value(value) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitSet(*this);
  }
  Expr *object;
  Token name;
  Expr *value;
};

class Super : public Expr
{
public:
  Super(Token keyword, Token method) : keyword(keyword), method(method) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitSuper(*this);
  }
  Token keyword;
  Token method;
};

class This : public Expr
{
public:
  This(Token keyword) : keyword(keyword) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitThis(*this);
  }
  Token keyword;
};

class Unary : public Expr
{
public:
  Unary(Token op, Expr *right) : op(op), right(right) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitUnary(*this);
  }
  Token op;
  Expr *right;
};

class Variable : public Expr
{
public:
  Variable(Token name) : name(name) {}
  std::string Accept(Visitor &visitor) override
  {
    return visitor.VisitVariable(*this);
  }
  Token name;
};

#endif // EXPR_H
