#ifndef VISIT_CALL_EXPR_H
#define VISIT_CALL_EXPR_H

#include "token.h"
#include "expr.h"
#include <chrono>
#include <vector>

class Interpreter;

class LoxCallable
{
public:
    virtual Object call(Interpreter *interpreter, std::vector<Object> arguments) = 0;
    virtual int arity() = 0;
};
#endif // VISIT_CALL_EXPR_H