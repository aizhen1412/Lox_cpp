#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

/*
 * File: lox_instance.h
 * ---------------------
 * This file defines the LoxInstance class, which represents an instance of a user-defined class in the Lox language.
 * Each LoxInstance has a reference to its class (of type LoxClass), and a map of fields that store the instance's variables.
 *
 * The LoxInstance class provides methods for setting and getting field values, and for converting the instance to a string.
 *
 * The Set method takes a token (representing the variable name) and an object (representing the value), and sets the field accordingly.
 *
 * The Get method takes a token (representing the variable name) and returns the corresponding value.
 *
 * The ToString method returns a string representation of the instance, which includes the class name and the instance's memory address.
 */
#include <string>
#include <unordered_map>
#include "lox_class.h"

class LoxInstance
{
public:
    LoxInstance(){};
    LoxInstance(LoxClass *klass);
    virtual ~LoxInstance(){};
    void Set(Token name, Object value);
    Object Get(Token name);
    std::string ToString();

private:
    LoxClass *klass;
    // An unordered_map storing the fields of the Lox instance.The key is the field name and the value is the field value.
    std::unordered_map<std::string, Object> fields;
};

#endif // LOXINSTANCE_H
