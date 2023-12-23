/*
 * visit_call_expr.h
 * This file defines the LoxCallable interface, which represents a callable object in the Lox language.
 * Each LoxCallable object must implement the Call method, which is used to call the object with a given list of arguments, and the Arity method, which returns the number of arguments that the object takes.
 */
#ifndef VISIT_CALL_EXPR_H
#define VISIT_CALL_EXPR_H

#include "token.h"
#include <vector>

class Interpreter;

class LoxCallable
{
public:
    virtual Object Call(Interpreter *interpreter, std::vector<Object> arguments) = 0;
    virtual int Arity() = 0;
    virtual ~LoxCallable() {}
};
#endif // VISIT_CALL_EXPR_H