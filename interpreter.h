#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include "ast_printer.h"
#include "expr.h"

class Interpreter : AstPrinter // 后面换成visitor
{
public:
    Object VisitLiteral(Literal &expr) override;
    Object VisitUnary(Unary &expr) override;
    Object VisitGrouping(Grouping &expr) override;
    Object VisitBinary(Binary &expr) override;
    void CheckNumberOperand(Token op, Object operand);
    void CheckNumberOperands(Token op, Object left, Object right);
    bool IsTruthy(Object object);
    bool IsEqual(Object a, Object b);
    Object Evaluate(Expr *expr);
    void Interpret(Expr *expression);
    std::string Stringify(Object object);
};

#endif // INTERPRETER_H