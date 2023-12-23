/*
 * lox_function.cpp
 * This file implements the LoxFunction class defined in lox_function.h.
 * The LoxFunction class represents a user-defined function in the Lox language.
 *
 * The constructor initializes the function with a Function declaration, an Environment pointer (representing the lexical environment where the function was defined),
 * and a boolean indicating whether it is an initializer of a class.
 *
 * The destructor deletes the closure environment.
 *
 * The Bind method is used for methods to bind the instance to the function's environment.
 *
 * The Call method executes the function with the given arguments. It creates a new environment for the function call, defines the function parameters in this environment,
 * and then executes the function body in this environment. If a return statement is encountered during execution, the function immediately returns the return value.
 * If the function is an initializer, it returns the instance ("this"). Otherwise, it returns null.
 *
 * The Arity method returns the number of parameters the function expects.
 *
 * The ToString method returns a string representation of the function.
 */
#include "lox_function.h"
#include "return_method.h"
#include "interpreter.h"

LoxFunction::LoxFunction(Function declaration, Environment *closure, bool isInitializer) : declaration(declaration), closure(closure), is_initializer(isInitializer) {}
LoxFunction::~LoxFunction()
{
    delete closure;
}
LoxFunction *LoxFunction::Bind(LoxInstance *instance)
{
    Environment *environment = new Environment(closure); // environment在loxfuction的析构函数中会被delete
    environment->Define("this", instance);
    return new LoxFunction(declaration, environment, is_initializer); // loxfunction在调用的函数中会被delete
}
Object LoxFunction::Call(Interpreter *interpreter, std::vector<Object> arguments)
{
    Environment *environment = new Environment(closure);
    for (std::vector<Token>::size_type i = 0; i < declaration.params.size(); i++)
    {
        environment->Define(declaration.params[i].lexeme, arguments[i]);
    }
    try
    {
        interpreter->ExecuteBlock(declaration.body, environment);
        environment->Set_enclosing(nullptr);
        delete environment;
    }
    catch (const Return_method &returnValue)
    {
        if (is_initializer)
            return closure->GetAt(0, "this");
        return returnValue.Get_value();
    }
    if (is_initializer)
        return closure->GetAt(0, "this");
    return nullptr;
}
int LoxFunction::Arity()
{
    return declaration.params.size();
}
std::string LoxFunction::ToString()
{
    return "<fn " + declaration.name.lexeme + ">";
}
