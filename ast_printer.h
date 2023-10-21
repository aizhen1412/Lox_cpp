// #ifndef AST_PRINTER_H
// #define AST_PRINTER_H

// #include <string>
// #include "expr.h"

// struct ToStringVisitor
// {
//     std::string operator()(const double &value) const;
//     std::string operator()(const std::string &value) const;
//     std::string operator()(std::nullptr_t) const;
//     std::string operator()(const bool &value) const;
//     std::string operator()(Expr *value) const;
// };

// class AstPrinter : public Visitor
// {
// public:
//     Object print(Expr &expr);
//     Object VisitBinary(Binary &expr) override;
//     Object VisitGrouping(Grouping &expr) override;
//     Object VisitUnary(Unary &expr) override;
//     Object VisitLiteral(Literal &expr) override;
//     // Object VisitCall(Call &Expr) override;
//     Object VisitAssignExpr(Assign &Expr) override;
//     Object VisitGet(Get &Expr) override;
//     Object VisitLogical(Logical &Expr) override;
//     Object VisitSet(Set &Expr) override;
//     Object VisitSuper(Super &Expr) override;
//     Object VisitThis(This &Expr) override;
//     Object VisitVariable(Variable &Expr) override;

//     Object visitBlockStmt(Block &stmt) override;
//     Object visitClassStmt(Class &stmt) override;
//     Object visitExpressionStmt(Expression &stmt) override;
//     Object visitFunctionStmt(Function &stmt) override;
//     Object visitIfStmt(If &stmt) override;
//     Object visitPrintStmt(Print &stmt) override;
//     Object visitReturnStmt(Return &stmt) override;
//     Object visitVarStmt(Var &stmt) override;
//     Object visitWhileStmt(While &stmt) override;

// private:
//     template <typename T, typename... Args>
//     std::string parenthesize(const T &name, Args... expr);
// };

// #endif // AST_PRINTER_H