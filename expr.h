/*
 * expr.h
 * This file defines the Expr, Stmt, and Visitor classes, which represent expressions, statements, and visitors in the Lox language.
 *
 * The Expr class is the base class for all expression classes. It has a virtual Accept method that takes a visitor and is overridden in each derived class.
 *
 * The Stmt class is the base class for all statement classes. It also has a virtual Accept method that takes a visitor and is overridden in each derived class.
 *
 * The Assign, Binary, Call, Get, Grouping, Literal, Logical, Set, Super, This, Unary, and Variable classes are derived from the Expr class. They represent different types of expressions in the Lox language. Each class has a constructor that initializes the expression with its operands, and an Accept method that accepts a visitor.
 *
 * The Block, Function, Class, Expression, If, Print, Return, Var, and While classes are derived from the Stmt class. They represent different types of statements in the Lox language. Each class has a constructor that initializes the statement with its components, and an Accept method that accepts a visitor.
 *
 * The Visitor class is a base class for all visitor classes. It has a virtual Visit... method for each type of expression and statement. These methods take an expression or statement and return an object.
 */
#ifndef EXPR_H
#define EXPR_H

#include <vector>
#include "token.h"
#include <iostream>

class Visitor;

class Expr
{
public:
  virtual ~Expr() {}
  virtual Object Accept(Visitor &visitor) = 0;
};
class Stmt
{
public:
  virtual ~Stmt(){};
  virtual Object Accept(Visitor &visitor) = 0;
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
  Block(std::vector<Stmt *> statements);
  Object Accept(Visitor &visitor) override;

  std::vector<Stmt *> statements;
};

class Function : public Stmt
{
public:
  Function() = default;
  Function(Token name, std::vector<Token> &params, std::vector<Stmt *> &body);

  Object Accept(Visitor &visitor);

  Token name;
  std::vector<Token> params;
  std::vector<Stmt *> body;
};

class Class : public Stmt
{
public:
  Class(Token name, Variable *superclass, std::vector<Function *> &methods);
  Object Accept(Visitor &visitor) override;

  Token name;
  Variable *superclass;
  std::vector<Function *> methods;
};

class Expression : public Stmt
{
public:
  Expression(Expr *expression);
  Object Accept(Visitor &visitor);

  Expr *expression;
};

class If : public Stmt
{
public:
  If(Expr *condition, Stmt *thenBranch, Stmt *elseBranch);
  Object Accept(Visitor &visitor);

  Expr *condition;
  Stmt *thenBranch;
  Stmt *elseBranch;
};

class Print : public Stmt
{
public:
  Print(Expr *expression);
  Object Accept(Visitor &visitor);

  Expr *expression;
};

class Return : public Stmt
{
public:
  Return(Token keyword, Expr *value);
  Object Accept(Visitor &visitor);

  Token keyword;
  Expr *value;
};

class Var : public Stmt
{
public:
  Var(Token name, Expr *initializer);
  Object Accept(Visitor &visitor);

  Token name;
  Expr *initializer;
};

class While : public Stmt
{
public:
  While(Expr *condition, Stmt *body);

  Object Accept(Visitor &visitor);
  Expr *condition;
  Stmt *body;
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

#endif // EXPR_H