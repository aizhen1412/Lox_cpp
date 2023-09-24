#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include "token.h"

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
  virtual Object Accept(Visitor &visitor) = 0;
};

class Visitor
{
public:
  virtual Object VisitAssign(Assign &Expr) = 0;
  virtual Object VisitBinary(Binary &Expr) = 0;
  virtual Object VisitCall(Call &Expr) = 0;
  virtual Object VisitGet(Get &Expr) = 0;
  virtual Object VisitGrouping(Grouping &Expr) = 0;
  virtual Object VisitLiteral(Literal &Expr) = 0;
  virtual Object VisitLogical(Logical &Expr) = 0;
  virtual Object VisitSet(Set &Expr) = 0;
  virtual Object VisitSuper(Super &Expr) = 0;
  virtual Object VisitThis(This &Expr) = 0;
  virtual Object VisitUnary(Unary &Expr) = 0;
  virtual Object VisitVariable(Variable &Expr) = 0;
};

class Assign : public Expr
{
public:
  Assign(Token name, Expr *value);
  Object Accept(Visitor &visitor) override;
  Token name;
  Expr *value;
};

class Binary : public Expr
{
public:
  Binary(Expr *left, Token op, Expr *right);
  Object Accept(Visitor &visitor) override;
  Expr *left;
  Token op;
  Expr *right;
};

class Call : public Expr
{
public:
  Call(Expr *callee, Token paren, std::vector<Expr *> arguments);
  Object Accept(Visitor &visitor) override;
  Expr *callee;
  Token paren;
  std::vector<Expr *> arguments;
};

class Get : public Expr
{
public:
  Get(Expr *object, Token name);
  Object Accept(Visitor &visitor) override;
  Expr *object;
  Token name;
};

class Grouping : public Expr
{
public:
  Grouping(Expr *expression);
  Object Accept(Visitor &visitor) override;
  Expr *expression;
};

class Literal : public Expr
{
public:
  Literal(Object value);
  Object Accept(Visitor &visitor) override;
  Object value;
};

class Logical : public Expr
{
public:
  Logical(Expr *left, Token op, Expr *right);
  Object Accept(Visitor &visitor) override;
  Expr *left;
  Token op;
  Expr *right;
};

class Set : public Expr
{
public:
  Set(Expr *object, Token name, Expr *value);
  Object Accept(Visitor &visitor) override;
  Expr *object;
  Token name;
  Expr *value;
};

class Super : public Expr
{
public:
  Super(Token keyword, Token method);
  Object Accept(Visitor &visitor) override;
  Token keyword;
  Token method;
};

class This : public Expr
{
public:
  This(Token keyword);
  Object Accept(Visitor &visitor) override;
  Token keyword;
};

class Unary : public Expr
{
public:
  Unary(Token op, Expr *right);
  Object Accept(Visitor &visitor) override;
  Token op;
  Expr *right;
};

class Variable : public Expr
{
public:
  Variable(Token name);
  Object Accept(Visitor &visitor) override;
  Token name;
};

#endif // EXPR_H
