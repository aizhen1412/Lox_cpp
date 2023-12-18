#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include "token.h"
#include <iostream>

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
  virtual Object Accept(Visitor &visitor) = 0;
};
class Visitor
{
public:
  virtual Object VisitAssignExpr(Assign &Expr) = 0;
  virtual Object VisitBinaryExpr(Binary &Expr) = 0;
  virtual Object VisitCallExpr(Call &Expr) = 0;
  virtual Object VisitGetExpr(Get &Expr) = 0;
  virtual Object VisitGroupingExpr(Grouping &Expr) = 0;
  virtual Object VisitLiteralExpr(Literal &Expr) = 0;
  virtual Object VisitLogicalExpr(Logical &Expr) = 0;
  virtual Object VisitSetExpr(Set &Expr) = 0;
  virtual Object VisitSuperExpr(Super &Expr) = 0;
  virtual Object VisitThisExpr(This &Expr) = 0;
  virtual Object VisitUnaryExpr(Unary &Expr) = 0;
  virtual Object VisitVariableExpr(Variable *Expr) = 0;

  virtual Object VisitBlockStmt(Block &stmt) = 0;
  virtual Object VisitClassStmt(Class &stmt) = 0;
  virtual Object VisitExpressionStmt(Expression &stmt) = 0;
  virtual Object VisitFunctionStmt(Function &stmt) = 0;
  virtual Object VisitIfStmt(If &stmt) = 0;
  virtual Object VisitPrintStmt(Print &stmt) = 0;
  virtual Object VisitReturnStmt(Return &stmt) = 0;
  virtual Object VisitVarStmt(Var &stmt) = 0;
  virtual Object VisitWhileStmt(While &stmt) = 0;
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
  Block(std::vector<Stmt *> statements) : statements(statements) {}
  virtual ~Block()
  {
    for (auto stmt : statements)
    {
      delete stmt;
    }
  }

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitBlockStmt(*this);
  }

  std::vector<Stmt *> statements;
};

class Class : public Stmt
{
public:
  Class(Token name, Variable *superclass, std::vector<Function *> &methods)
      : name(name), superclass(superclass), methods(methods) {}

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitClassStmt(*this);
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

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitExpressionStmt(*this);
  }

  Expr *expression;
};

class Function : public Stmt
{
public:
  Function() = default;
  Function(Token name, std::vector<Token> &params, std::vector<Stmt *> &body) : name(name), params(params), body(body) {}

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitFunctionStmt(*this);
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

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitIfStmt(*this);
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

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitPrintStmt(*this);
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

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitReturnStmt(*this);
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

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitVarStmt(*this);
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

  Object Accept(Visitor &visitor) override
  {
    return visitor.VisitWhileStmt(*this);
  }

  Expr *condition;
  Stmt *body;
};

#endif // EXPR_H