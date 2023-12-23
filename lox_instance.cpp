/*
 * lox_instance.cpp
 * This file implements the LoxInstance class defined in lox_instance.h.
 * The LoxInstance class represents an instance of a user-defined class in the Lox language.
 * Each LoxInstance has a reference to its class (of type LoxClass), and a map of fields that store the instance's variables.
 *
 * The Set method sets the value of a field.
 * The Get method returns the value of a field, or throws a RuntimeError if the field is not defined.
 * The ToString method returns a string representation of the instance.
 */
#include "lox_instance.h"
#include "runtime_error.h"

LoxInstance::LoxInstance(LoxClass *klass) : klass(klass) {}

void LoxInstance::Set(Token name, Object value)
{
    fields[name.lexeme] = value;
}
Object LoxInstance::Get(Token name)
{
    if (fields.find(name.lexeme) != fields.end())
        return fields[name.lexeme];

    LoxFunction *method = klass->FindMethod(name.lexeme);

    if (method != nullptr)
        return method->Bind(this);

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}
std::string LoxInstance::ToString()
{
    return klass->Get_name() + " instance";
}
