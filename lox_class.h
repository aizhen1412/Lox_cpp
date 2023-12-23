/*
 * lox_class.h
 * This file defines the LoxClass class, which represents a user-defined class in the Lox language.
 * Each LoxClass has a name, a superclass, and a map of methods.
 *
 * The LoxClass class provides methods for finding a method by name, calling the class (which creates a new instance), getting the arity (which is always zero for classes),
 * getting the class name, and converting the class to a string.
 *
 * The FindMethod method returns the method with the given name, or null if the method is not found.
 * The Call method creates a new instance of the class and calls the initializer method, if it exists.
 * The Arity method returns zero, because classes are called without arguments to create a new instance.
 * The Get_name method returns the name of the class.
 * The ToString method returns a string representation of the class.
 */
#ifndef LOXCLASS_H
#define LOXCLASS_H

#include <string>
#include <unordered_map>
#include "visit_call_expr.h"
#include "lox_function.h"

class LoxFunction;

class LoxClass : public LoxCallable
{
public:
    LoxClass() = default;
    LoxClass(std::string name, LoxClass *superclass, std::unordered_map<std::string, LoxFunction *> methods);
    virtual ~LoxClass();
    // returns the method with the given name, or null if the method is not found.
    LoxFunction *FindMethod(std::string name);
    // creates a new instance of the class and calls the initializer method, if it exists.
    Object Call(Interpreter *interpreter, std::vector<Object> arguments) override;
    // return the number of parameters the initializer method expects, or zero if the initializer method does not exist.
    int Arity() override;
    // returns the name of the class.
    std::string Get_name();
    // returns a string representation of the class.
    std::string ToString();

private:
    std::string name;                                       // the name of the class
    LoxClass *superclass;                                   // the superclass of the class
    std::unordered_map<std::string, LoxFunction *> methods; // the methods of the class
};

#endif // LOXCLASS_H