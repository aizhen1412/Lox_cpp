/*
 * File: ast_printer.cpp
 * ----------------------
 * This file implements the ToStringVisitor struct defined in ast_printer.h.
 *
 * The ToStringVisitor struct is a visitor struct that is used to convert different types of values to strings.
 * It overloads the function call operator for different types of values.
 *
 * The operator() methods are used to convert the corresponding type of value to a string.
 * For example, the operator()(const double &value) method converts a double value to a string,
 * and the operator()(const bool &value) method converts a boolean value to a string.
 *
 * This file is part of the implementation of a programming language interpreter.
 */
#include <sstream>
#include "ast_printer.h"

std::string ToStringVisitor::operator()(const double &value) const
{
    return std::to_string(value);
}
std::string ToStringVisitor::operator()(const std::string &value) const
{
    return value;
}
std::string ToStringVisitor::operator()(std::nullptr_t) const
{
    return "nil";
}
std::string ToStringVisitor::operator()(const bool &value) const
{
    return value ? "true_" : "false";
}
std::string ToStringVisitor::operator()(Expr *value) const
{
    return "Expr";
}
void AstPrinter::print(Stmt *expr)
{
    if (std::holds_alternative<std::string>(expr->Accept(*this)))
        std::cout << std::get<std::string>(expr->Accept(*this)) << std::endl;
}
void AstPrinter::print(Expr *expr)
{
    if (std::holds_alternative<std::string>(expr->Accept(*this)))
        std::cout << std::get<std::string>(expr->Accept(*this)) << std::endl;
}
Object AstPrinter::VisitBinaryExpr(Binary &expr)
{
    return parenthesize(expr.op.lexeme, expr.left, expr.right);
}
Object AstPrinter::VisitGroupingExpr(Grouping &expr)
{
    return parenthesize("group", expr.expression);
}
Object AstPrinter::VisitUnaryExpr(Unary &expr)
{
    return parenthesize(expr.op.lexeme, expr.right);
}
Object AstPrinter::VisitLiteralExpr(Literal &expr)
{
    return parenthesize(std::visit(ToStringVisitor(), expr.value));
}
Object AstPrinter::VisitAssignExpr(Assign &expr)
{
    return parenthesize(expr.name.lexeme, expr.value);
}
Object AstPrinter::VisitGetExpr(Get &expr)
{
    return parenthesize(expr.name.lexeme, expr.object);
}
Object AstPrinter::VisitLogicalExpr(Logical &expr)
{
    return parenthesize(expr.op.lexeme, expr.left, expr.right);
}
Object AstPrinter::VisitSetExpr(Set &expr)
{
    return parenthesize(expr.name.lexeme, expr.value);
}
Object AstPrinter::VisitSuperExpr(Super &expr)
{
    std::string ret = "Super expression " + expr.method.lexeme;
    return ret;
}
Object AstPrinter::VisitThisExpr(This &expr)
{
    std::string ret = "This expression " + expr.keyword.lexeme;
    return ret;
}
Object AstPrinter::VisitCallExpr(Call &expr)
{
    return parenthesize("Call ", expr.callee);
}
Object AstPrinter::VisitVariableExpr(Variable &expr)
{
    return parenthesize(expr.name.lexeme);
}
Object AstPrinter::VisitBlockStmt(Block &stmt)
{
    return parenthesize_fun("Block", stmt.statements);
}
Object AstPrinter::VisitClassStmt(Class &stmt)
{
    return parenthesize_fun("Class " + stmt.name.lexeme, stmt.methods);
}
Object AstPrinter::VisitExpressionStmt(Expression &stmt)
{
    Object ret = stmt.expression->Accept(*this);
    return ret;
}
Object AstPrinter::VisitFunctionStmt(Function &stmt)
{
    return parenthesize_fun("Fun " + stmt.name.lexeme, stmt.body);
}
Object AstPrinter::VisitIfStmt(If &stmt)
{
    if (stmt.elseBranch != nullptr)
        return parenthesize("If", stmt.condition, stmt.thenBranch, stmt.elseBranch);
    else
        return parenthesize("If", stmt.condition, stmt.thenBranch);
}
Object AstPrinter::VisitPrintStmt(Print &stmt)
{
    return parenthesize("Print", stmt.expression);
}
Object AstPrinter::VisitReturnStmt(Return &stmt)
{
    return parenthesize(stmt.keyword.lexeme, stmt.value);
}
Object AstPrinter::VisitVarStmt(Var &stmt)
{
    return parenthesize(stmt.name.lexeme, stmt.initializer);
}
Object AstPrinter::VisitWhileStmt(While &stmt)
{
    return parenthesize("While", stmt.condition, stmt.body);
}

template <typename T, typename... Args>
std::string AstPrinter::parenthesize(const T &name, Args... expr)
{
    std::stringstream ss;
    ss << "(" << name;

    ((ss << " " << std::get<std::string>(expr->Accept(*this))), ...);

    ss << ")";

    return ss.str();
}
std::string AstPrinter::parenthesize_fun(std::string name, const std::vector<Stmt *> &body)
{
    std::stringstream ss;
    ss << "(" << name;

    for (const auto &statement : body)
        ss << " " << std::get<std::string>(statement->Accept(*this));

    ss << ")";
    return ss.str();
}
std::string AstPrinter::parenthesize_fun(std::string name, const std::vector<Function *> &body)
{
    std::stringstream ss;
    ss << "(" << name;

    for (const auto &statement : body)
        ss << " " << std::get<std::string>(statement->Accept(*this));

    ss << ")";
    return ss.str();
}