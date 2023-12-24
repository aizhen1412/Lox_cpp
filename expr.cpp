/*
 * expr.cpp
 * This file implements the Expr and Stmt classes defined in expr.h.
 * The Expr and Stmt classes represent expressions and statements in the Lox language.
 *
 * The Assign, Binary, Call, Get, Grouping, Literal, Logical, Set, Super, This, Unary, and Variable classes are derived from the Expr class. They represent different types of expressions in the Lox language. Each class has a constructor that initializes the expression with its operands, and an Accept method that accepts a visitor and calls the appropriate Visit... method on it.
 *
 * The Block, Function, Class, Expression, If, Print, Return, Var, and While classes are derived from the Stmt class. They represent different types of statements in the Lox language. Each class has a constructor that initializes the statement with its components, and an Accept method that accepts a visitor and calls the appropriate Visit... method on it.
 */

#include "expr.h"
#include <iostream>
#include <typeinfo>
#include "interpreter.h"
#include "resolver.h"

Assign::Assign(Token name, Expr *value) : name(name), value(value) {}
Object Assign::Accept(Visitor &visitor) { return visitor.VisitAssignExpr(*this); }

Binary::Binary(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
Object Binary::Accept(Visitor &visitor) { return visitor.VisitBinaryExpr(*this); }

Call::Call(Expr *callee, Token paren, std::vector<Expr *> arguments) : callee(callee), paren(paren), arguments(arguments) {}
Object Call::Accept(Visitor &visitor) { return visitor.VisitCallExpr(*this); }

Get::Get(Expr *object, Token name) : object(object), name(name) {}
Object Get::Accept(Visitor &visitor) { return visitor.VisitGetExpr(*this); }

Grouping::Grouping(Expr *expression) : expression(expression) {}
Object Grouping::Accept(Visitor &visitor) { return visitor.VisitGroupingExpr(*this); }

Literal::Literal(Object value) : value(value) {}
Object Literal::Accept(Visitor &visitor) { return visitor.VisitLiteralExpr(*this); }

Logical::Logical(Expr *left, Token op, Expr *right) : left(left), op(op), right(right) {}
Object Logical::Accept(Visitor &visitor) { return visitor.VisitLogicalExpr(*this); }

Set::Set(Expr *object, Token name, Expr *value) : object(object), name(name), value(value) {}
Object Set::Accept(Visitor &visitor) { return visitor.VisitSetExpr(*this); }

Super::Super(Token keyword, Token method) : keyword(keyword), method(method) {}
Object Super::Accept(Visitor &visitor) { return visitor.VisitSuperExpr(*this); }

This::This(Token keyword) : keyword(keyword) {}
Object This::Accept(Visitor &visitor) { return visitor.VisitThisExpr(*this); }

Unary::Unary(Token op, Expr *right) : op(op), right(right) {}
Object Unary::Accept(Visitor &visitor) { return visitor.VisitUnaryExpr(*this); }

Variable::Variable(Token name) : name(name) {}
Object Variable::Accept(Visitor &visitor) { return visitor.VisitVariableExpr(*this); }

Block::Block(std::vector<Stmt *> statements) : statements(statements) {}
Object Block::Accept(Visitor &visitor) { return visitor.VisitBlockStmt(*this); }

Function::Function(Token name, std::vector<Token> &params, std::vector<Stmt *> &body) : name(name), params(params), body(body) {}
Object Function::Accept(Visitor &visitor) { return visitor.VisitFunctionStmt(*this); }

Class::Class(Token name, Variable *superclass, std::vector<Function *> &methods) : name(name), superclass(superclass), methods(methods) {}
Object Class::Accept(Visitor &visitor) { return visitor.VisitClassStmt(*this); }

Expression::Expression(Expr *expression) : expression(expression) {}
Object Expression::Accept(Visitor &visitor) { return visitor.VisitExpressionStmt(*this); }

If::If(Expr *condition, Stmt *thenBranch, Stmt *elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
Object If::Accept(Visitor &visitor) { return visitor.VisitIfStmt(*this); }

Print::Print(Expr *expression) : expression(expression) {}
Object Print::Accept(Visitor &visitor) { return visitor.VisitPrintStmt(*this); }

Return::Return(Token keyword, Expr *value) : keyword(keyword), value(value) {}
Object Return::Accept(Visitor &visitor) { return visitor.VisitReturnStmt(*this); }

Var::Var(Token name, Expr *initializer) : name(name), initializer(initializer) {}
Object Var::Accept(Visitor &visitor) { return visitor.VisitVarStmt(*this); }

While::While(Expr *condition, Stmt *body) : condition(condition), body(body) {}
Object While::Accept(Visitor &visitor) { return visitor.VisitWhileStmt(*this); }