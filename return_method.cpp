/*
 * return_method.cpp
 * This file implements the Return_method class defined in return_method.h.
 * The Return_method class represents the result of a return statement in the Lox language.
 * Each Return_method has a value, which is the value returned by the return statement.
 * The Return_method class includes a constructor for creating a new Return_method, and a method for getting the value of the return statement.
 */
#include "return_method.h"

Return_method::Return_method(Object value)
{
    this->value = value;
}
Object Return_method::Get_value() const
{
    return this->value;
}