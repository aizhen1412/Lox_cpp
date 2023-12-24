/*
 * interpreter.h
 * This file defines the Interpreter class, which is the core of the Lox language. It interprets and executes Lox code.
 * The Interpreter class inherits from the Visitor class, which is used to visit different types of expressions and statements.
 *
 * The Interpreter class provides methods for interpreting a list of statements, executing a block of statements, resolving variables,
 * visiting different types of expressions and statements, checking if operands are numbers, checking if an object is truthy,
 * checking if two objects are equal, evaluating expressions, executing statements, looking up variables, and converting objects to strings.
 *
 * The Interpret method is the entry point of the interpreter. It takes a list of statements and interprets them.
 *
 * The ExecuteBlock method executes a block of statements in a given environment.
 *
 * The Resolve method resolves a variable and stores its depth in the locals map.
 *
 * The Visit... methods are used to visit different types of expressions and statements. They override the methods defined in the Visitor class.
 *
 * The CheckNumberOperand and CheckNumberOperands methods check if the operand(s) of an operation are numbers.
 *
 * The IsTruthy method checks if an object is truthy according to the rules of the Lox language.
 *
 * The IsEqual method checks if two objects are equal.
 *
 * The Evaluate method evaluates an expression and returns its value.
 *
 * The Execute method executes a statement.
 *
 * The LookUpVariable method looks up a variable in the environment.
 *
 * The Stringify method converts an object to a string.
 */
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <memory>
#include "ast_printer.h"
#include "expr.h"
#include "environment.h"
#include "lox_function.h"
#include "error.h"
#include "lox_class.h"

class Interpreter : public Visitor // 后面换成visitor
{
public:
    Interpreter() = default;
    ~Interpreter();
    // entry point of the interpreter
    void Interpret(std::vector<Stmt *> statements);
    // executes a block of statements in a given environment
    void ExecuteBlock(std::vector<Stmt *> statements, Environment *environment);
    // resolves a variable and stores its depth in the locals map
    void Resolve(Expr *expr, int depth);

private:
    Environment *globals = new Environment();
    Environment *environment = globals;
    std::unordered_map<Expr *, int> locals;
    // visitor methods
    Object VisitSuperExpr(Super &Expr) override;
    Object VisitLiteralExpr(Literal &expr) override;
    Object VisitLogicalExpr(Logical &expr) override;
    Object VisitGetExpr(Get &Expr) override;
    Object VisitSetExpr(Set &Expr) override;
    Object VisitThisExpr(This &expr);
    Object VisitUnaryExpr(Unary &expr) override;
    Object VisitVariableExpr(Variable &expr) override;
    Object VisitGroupingExpr(Grouping &expr) override;
    Object VisitBinaryExpr(Binary &expr) override;
    Object VisitCallExpr(Call &expr);
    // check if the operand(s) of an operation are numbers
    void CheckNumberOperand(Token op, Object operand);
    void CheckNumberOperands(Token op, Object left, Object right);

    bool IsTruthy(Object object);
    bool IsEqual(Object a, Object b);
    // evaluate an expression and return its value
    Object Evaluate(Expr *expr);
    // execute a statement
    void Execute(Stmt *stmt);
    // look up a variable in the environment
    Object LookUpVariable(Token name, Expr *expr);
    // visit methods
    Object VisitBlockStmt(Block &stmt) override;
    Object VisitClassStmt(Class &stmt) override;
    Object VisitExpressionStmt(Expression &stmt) override;
    Object VisitFunctionStmt(Function &stmt);
    Object VisitIfStmt(If &stmt);
    Object VisitPrintStmt(Print &stmt) override;
    Object VisitReturnStmt(Return &stmt);
    Object VisitVarStmt(Var &stmt) override;
    Object VisitWhileStmt(While &stmt) override;
    Object VisitAssignExpr(Assign &expr);
    // convert an object to a string
    std::string Stringify(Object object);
};

#endif // INTERPRETER_H