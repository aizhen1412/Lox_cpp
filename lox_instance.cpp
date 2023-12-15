#include "lox_instance.h"
#include "runtime_error.h"
#include <iostream>
LoxInstance::LoxInstance(LoxClass *klass) : klass(klass)
{
}
Object LoxInstance::get(Token name)
{
    if (fields.find(name.lexeme) != fields.end())
    {
        return fields[name.lexeme];
    }
    LoxFunction *method = klass->findMethod(name.lexeme);
    if (method != nullptr)
        return method->bind(this);
    throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}
std::string LoxInstance::toString()
{

    return klass->name + " instance";
}
