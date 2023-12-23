/*
 * lox_function.h
 * This file defines the LoxFunction class, which represents a user-defined function in the Lox language.
 * Each LoxFunction has a Function declaration, an Environment pointer (representing the lexical environment where the function was defined),
 * and a boolean indicating whether it is an initializer of a class.
 *
 * The LoxFunction class provides methods for binding an instance (for methods), calling the function, getting the arity (number of parameters),
 * and converting the function to a string.
 *
 * The Bind method is used for methods to bind the instance to the function's environment.
 * The Call method executes the function with the given arguments.
 * The Arity method returns the number of parameters the function expects.
 * The ToString method returns a string representation of the function.
 */
#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include "visit_call_expr.h"
#include "environment.h"
#include "expr.h"

class Interpreter;

class LoxFunction : public LoxCallable
{
public:
    LoxFunction() = default;
    LoxFunction(Function declaration, Environment *closure, bool isInitializer);
    ~LoxFunction();
    // used for methods to bind the instance to the function's environment.
    LoxFunction *Bind(LoxInstance *instance);
    // executes the function with the given arguments.
    Object Call(Interpreter *interpreter, std::vector<Object> arguments);
    // returns the number of parameters the function expects.
    int Arity();

private:
    Function declaration; // the function declaration
    Environment *closure; // the lexical environment where the function was defined
    bool is_initializer;  // whether it is an initializer of a class
    // returns a string representation of the function.
    std::string ToString();
};
#endif // LOX_FUNCTION_H