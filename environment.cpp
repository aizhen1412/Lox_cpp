/*
 * environment.cpp
 * This file implements the Environment class defined in environment.h.
 * The Environment class is used to store and manage variables in the Lox language.
 *
 * The constructors initialize the environment with an optional enclosing environment.
 *
 * The destructor deletes the enclosing environment and the values of the variables in the environment.
 *
 * The Get method takes a token representing a variable's name and returns the variable's value. If the variable is not found in the environment, it looks for the variable in the enclosing environment. If the variable is still not found, it throws a RuntimeError.
 *
 * The GetAt method takes a distance and a variable's name, and returns the variable's value from the ancestor environment at the given distance.
 *
 * The Define method takes a variable's name and a value, and defines the variable in the environment with the given value.
 *
 * The Assign method takes a token representing a variable's name and a value, and assigns the value to the variable in the environment. If the variable is not found in the environment, it looks for the variable in the enclosing environment. If the variable is still not found, it throws a RuntimeError.
 *
 * The AssignAt method takes a distance, a token representing a variable's name, and a value, and assigns the value to the variable in the ancestor environment at the given distance.
 *
 * The Ancestor method takes a distance and returns the ancestor environment at the given distance.
 *
 * The Get_enclosing and Set_enclosing methods are used to get and set the enclosing environment.
 *
 * The values map stores the values of the variables in the environment.
 */
#include <iostream>
#include "environment.h"
#include "error.h"

Environment::Environment()
{
    this->enclosing = nullptr;
}
Environment::Environment(Environment *enclosing)
{
    this->enclosing = enclosing;
}
Environment::~Environment()
{
    // delete enclosing; //may cause double free
    for (auto it = values.begin(); it != values.end(); it++)
    {
        std::visit([](auto &&arg)
                   { using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_pointer_v<T>)
                    {
                        delete arg;
                    } },
                   it->second);
    }
}
Object Environment::Get(Token name)
{
    if (values.find(name.lexeme) != values.end())
    {
        return values[name.lexeme];
    }
    if (enclosing != nullptr)
        return enclosing->Get(name);
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
Object Environment::GetAt(int distance, std::string name)
{
    return Ancestor(distance)->values[name];
}
void Environment::Define(const std::string &name, Object value)
{
    values[name] = value;
}
void Environment::Assign(const Token &name, Object value)
{
    auto it = values.find(name.lexeme);
    if (it != values.end())
    {
        it->second = value;
        return;
    }
    if (enclosing != nullptr)
    {
        enclosing->Assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
void Environment::AssignAt(int distance, Token name, Object value)
{
    Ancestor(distance)->values[name.lexeme] = value;
}
Environment *Environment::Ancestor(int distance)
{
    Environment *environment = this;
    for (int i = 0; i < distance; i++)
    {
        environment = environment->enclosing;
    }

    return environment;
}
Environment *Environment::Get_enclosing()
{
    return enclosing;
}
Environment *Environment::Set_enclosing(Environment *enclosing)
{
    this->enclosing = enclosing;
    return enclosing;
}