/*
 * return_method.h
 * This file defines the Return_method class, which represents the result of a return statement in the Lox language.
 * Each Return_method has a value, which is the value returned by the return statement.
 * The Return_method class includes a constructor for creating a new Return_method, and a method for getting the value of the return statement.
 */
#ifndef RETURN_METHOD_H
#define RETURN_METHOD_H

#include "token.h"

class Return_method
{
public:
    Return_method(Object value);
    Object Get_value() const;

private:
    Object value;
};
#endif // RETURN_METHOD_H