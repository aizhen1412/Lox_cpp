/*
 * File: ast_printer.h
 * -------------------
 * This file defines the AstPrinter class and the ToStringVisitor struct.
 *
 * The AstPrinter class is a visitor class that is used to print the abstract syntax tree (AST) of a program.
 * It overrides the visit methods for each type of expression (Expr) and statement (Stmt) in the AST.
 * The print methods are used to initiate the printing process for an expression or a statement.
 * The parenthesize methods are helper methods used to format the output.
 *
 * The ToStringVisitor struct is a visitor struct that is used to convert different types of values to strings.
 * It overloads the function call operator for different types of values.
 *
 * This file is part of the implementation of a programming language interpreter.
 */
#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <string>
#include "expr.h"

struct ToStringVisitor
{
    std::string operator()(const double &value) const;
    std::string operator()(const std::string &value) const;
    std::string operator()(std::nullptr_t) const;
    std::string operator()(const bool &value) const;
    std::string operator()(Expr *value) const;
};

class AstPrinter : public Visitor
{
public:
    void print(Stmt *expr);
    void print(Expr *expr);

private:
    Object VisitBinaryExpr(Binary &expr) override;
    Object VisitGroupingExpr(Grouping &expr) override;
    Object VisitLiteralExpr(Literal &expr) override;
    Object VisitUnaryExpr(Unary &expr) override;
    Object VisitAssignExpr(Assign &expr) override;
    Object VisitGetExpr(Get &expr) override;
    Object VisitLogicalExpr(Logical &expr) override;
    Object VisitSetExpr(Set &expr) override;
    Object VisitSuperExpr(Super &expr) override;
    Object VisitThisExpr(This &expr) override;
    Object VisitCallExpr(Call &expr) override;
    Object VisitVariableExpr(Variable &expr) override;
    Object VisitBlockStmt(Block &stmt) override;
    Object VisitClassStmt(Class &stmt) override;
    Object VisitExpressionStmt(Expression &stmt) override;
    Object VisitFunctionStmt(Function &stmt) override;
    Object VisitIfStmt(If &stmt) override;
    Object VisitPrintStmt(Print &stmt) override;
    Object VisitReturnStmt(Return &stmt) override;
    Object VisitVarStmt(Var &stmt) override;
    Object VisitWhileStmt(While &stmt) override;

    template <typename T, typename... Args>
    std::string parenthesize(const T &name, Args... expr);
    std::string parenthesize_fun(std::string name, const std::vector<Stmt *> &body);
    std::string parenthesize_fun(std::string name, const std::vector<Function *> &body);
};

#endif // AST_PRINTER_H