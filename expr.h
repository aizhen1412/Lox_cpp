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

class Block;
class Class;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;
class While;

class Expr
{
public:
  virtual ~Expr() {}
  virtual Object Accept(Visitor &visitor) = 0;
};
class Stmt
{
public:
  virtual ~Stmt() = default;
  virtual Object accept(Visitor &visitor) = 0;
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

  virtual Object visitBlockStmt(Block &stmt) = 0;
  virtual Object visitClassStmt(Class &stmt) = 0;
  virtual Object visitExpressionStmt(Expression &stmt) = 0;
  virtual Object visitFunctionStmt(Function &stmt) = 0;
  virtual Object visitIfStmt(If &stmt) = 0;
  virtual Object visitPrintStmt(Print &stmt) = 0;
  virtual Object visitReturnStmt(Return &stmt) = 0;
  virtual Object visitVarStmt(Var &stmt) = 0;
  virtual Object visitWhileStmt(While &stmt) = 0;
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

class Block : public Stmt
{
public:
  Block(std::vector<Stmt *> &statements) : statements(statements) {}
  virtual ~Block()
  {
    for (auto stmt : statements)
    {
      delete stmt;
    }
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitBlockStmt(*this);
  }

  std::vector<Stmt *> statements;
};

class Class : public Stmt
{
public:
  Class(Token name, Variable *superclass, std::vector<Function *> &methods)
      : name(name), superclass(superclass), methods(methods) {}

  Object accept(Visitor &visitor) override
  {
    return visitor.visitClassStmt(*this);
  }

  Token name;
  Variable *superclass;
  std::vector<Function *> methods;
};

class Expression : public Stmt
{
public:
  Expression(Expr *expression) : expression(expression) {}
  virtual ~Expression()
  {
    delete expression;
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitExpressionStmt(*this);
  }

  Expr *expression;
};

class Function : public Stmt
{
public:
  Function(Token name, std::vector<Token> &params, std::vector<Stmt *> &body) : name(name), params(params), body(body) {}

  virtual ~Function()
  {
    for (auto stmt : body)
    {
      delete stmt;
    }
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitFunctionStmt(*this);
  }

  Token name;
  std::vector<Token> params;
  std::vector<Stmt *> body;
};

class If : public Stmt
{
public:
  If(Expr *condition, Stmt *thenBranch, Stmt *elseBranch)
      : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

  virtual ~If()
  {
    delete condition;
    delete thenBranch;
    if (elseBranch)
      delete elseBranch;
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitIfStmt(*this);
  }

  Expr *condition;
  Stmt *thenBranch;
  Stmt *elseBranch;
};

class Print : public Stmt
{
public:
  Print(Expr *expression) : expression(expression) {}
  virtual ~Print()
  {
    delete expression;
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitPrintStmt(*this);
  }

  Expr *expression;
};

class Return : public Stmt
{
public:
  Return(Token keyword, Expr *value) : keyword(keyword), value(value) {}
  virtual ~Return()
  {
    delete value;
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitReturnStmt(*this);
  }

  Token keyword;
  Expr *value;
};

class Var : public Stmt
{
public:
  Var(Token name, Expr *initializer) : name(name), initializer(initializer) {}
  virtual ~Var()
  {
    delete initializer;
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitVarStmt(*this);
  }

  Token name;
  Expr *initializer;
};

class While : public Stmt
{
public:
  While(Expr *condition, Stmt *body) : condition(condition), body(body) {}
  virtual ~While()
  {
    delete condition;
    delete body;
  }

  Object accept(Visitor &visitor) override
  {
    return visitor.visitWhileStmt(*this);
  }

  Expr *condition;
  Stmt *body;
};

#endif // EXPR_H
