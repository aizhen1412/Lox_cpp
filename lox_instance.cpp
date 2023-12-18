#include "lox_instance.h"
#include "runtime_error.h"
#include <iostream>
LoxInstance::LoxInstance(LoxClass *klass) : klass(klass)
{
}
Object LoxInstance::Get(Token name)
{
    if (fields.find(name.lexeme) != fields.end())
    {
        return fields[name.lexeme];
    }
    LoxFunction *method = klass->FindMethod(name.lexeme);
    if (method != nullptr)
        return method->Bind(this);
    throw new RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}
void LoxInstance::Set(Token name, Object value)
{
    fields[name.lexeme] = value;
}
std::string LoxInstance::ToString()
{

    return klass->name + " instance";
}
