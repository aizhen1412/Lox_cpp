/*
 * Environment.h
 * This file defines the Environment class, which is used to store and manage variables in the Lox language.
 * The Environment class provides methods for getting a variable's value, defining a variable, assigning a value to a variable, and managing enclosing environments.
 *
 * The constructor initializes the environment with an optional enclosing environment.
 *
 * The destructor deletes the environment.
 *
 * The Get method takes a token representing a variable's name and returns the variable's value. If the variable is not found, it throws a RuntimeError.
 *
 * The GetAt method takes a distance and a variable's name, and returns the variable's value from the ancestor environment at the given distance.
 *
 * The Define method takes a variable's name and a value, and defines the variable in the environment with the given value.
 *
 * The Assign method takes a token representing a variable's name and a value, and assigns the value to the variable in the environment. If the variable is not found, it throws a RuntimeError.
 *
 * The AssignAt method takes a distance, a token representing a variable's name, and a value, and assigns the value to the variable in the ancestor environment at the given distance.
 *
 * The Ancestor method takes a distance and returns the ancestor environment at the given distance.
 *
 * The Get_enclosing and Set_enclosing methods are used to get and set the enclosing environment.
 *
 * The values map stores the values of the variables in the environment.
 */
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "token.h"

class Environment
{
public:
    Environment();
    Environment(Environment *enclosing); // initializes the environment with an optional enclosing environment.
    ~Environment();                      // deletes the environment.

    // takes a token representing a variable's name and returns the variable's value. If the variable is not found, it throws a RuntimeError.
    Object Get(Token name);
    // takes a distance and a variable's name, and returns the variable's value from the ancestor environment at the given distance.
    Object GetAt(int distance, std::string name);
    // takes a variable's name and a value, and defines the variable in the environment with the given value.
    void Define(const std::string &name, Object value);
    // takes a token representing a variable's name and a value, and assigns the value to the variable in the environment. If the variable is not found, it throws a RuntimeError.
    void Assign(const Token &name, Object value);
    // takes a distance, a token representing a variable's name, and a value, and assigns the value to the variable in the ancestor environment at the given distance.
    void AssignAt(int distance, Token name, Object value);
    // takes a distance and returns the ancestor environment at the given distance.
    Environment *Ancestor(int distance);
    // used to get and set the enclosing environment.
    Environment *Get_enclosing();
    Environment *Set_enclosing(Environment *enclosing);

private:
    Environment *enclosing;
    std::unordered_map<std::string, Object> values;
};

#endif // ENVIRONMENT_H