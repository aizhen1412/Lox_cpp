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

class Expr // 表达式
{
public:
  virtual ~Expr() {}
  virtual Object Accept(Visitor &visitor) = 0;
};
class Stmt // 语句
{
public:
  virtual ~Stmt() = default;
  virtual Object Accept(Visitor &visitor) = 0;
};
class Visitor // 访问者
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

class Assign : public Expr // 赋值表达式
{
public:
  Assign(Token name, Expr *value);
  Object Accept(Visitor &visitor) override;
  Token name;
  Expr *value;
};

class Binary : public Expr // 二元表达式
{
public:
  Binary(Expr *left, Token op, Expr *right);
  Object Accept(Visitor &visitor) override;
  Expr *left;
  Token op;
  Expr *right;
};

class Call : public Expr // 函数调用表达式
{
public:
  Call(Expr *callee, Token paren, std::vector<Expr *> arguments);
  Object Accept(Visitor &visitor) override;
  Expr *callee;
  Token paren;
  std::vector<Expr *> arguments;
};

class Get : public Expr // 获取属性表达式
{
public:
  Get(Expr *object, Token name);
  Object Accept(Visitor &visitor) override;
  Expr *object;
  Token name;
};

class Grouping : public Expr // 分组表达式
{
public:
  Grouping(Expr *expression);
  Object Accept(Visitor &visitor) override;
  Expr *expression;
};

class Literal : public Expr // 字面量表达式
{
public:
  Literal(Object value);
  Object Accept(Visitor &visitor) override;
  Object value;
};

class Logical : public Expr // 逻辑表达式
{
public:
  Logical(Expr *left, Token op, Expr *right);
  Object Accept(Visitor &visitor) override;
  Expr *left;
  Token op;
  Expr *right;
};

class Set : public Expr // 设置属性表达式
{
public:
  Set(Expr *object, Token name, Expr *value);
  Object Accept(Visitor &visitor) override;
  Expr *object;
  Token name;
  Expr *value;
};

class Super : public Expr // super表达式
{
public:
  Super(Token keyword, Token method);
  Object Accept(Visitor &visitor) override;
  Token keyword;
  Token method;
};

class This : public Expr // this表达式
{
public:
  This(Token keyword);
  Object Accept(Visitor &visitor) override;
  Token keyword;
};

class Unary : public Expr // 一元表达式
{
public:
  Unary(Token op, Expr *right);
  Object Accept(Visitor &visitor) override;
  Token op;
  Expr *right;
};

class Variable : public Expr // 变量表达式
{
public:
  Variable(Token name);
  Object Accept(Visitor &visitor) override;
  Token name;
};

class Block : public Stmt // 代码块
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

class Class : public Stmt // 类
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

class Expression : public Stmt // 表达式语句
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

class Function : public Stmt // 函数
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

class If : public Stmt // if语句
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

class Print : public Stmt // print语句
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

class Return : public Stmt // return语句
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

class Var : public Stmt // var语句
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

class While : public Stmt // while语句
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