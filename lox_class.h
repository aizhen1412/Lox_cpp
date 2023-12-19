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
    std::string name;
    LoxClass *superclass;
    std::unordered_map<std::string, LoxFunction *> methods;
    LoxClass(std::string name, LoxClass *superclass, std::unordered_map<std::string, LoxFunction *> methods);

    LoxFunction *FindMethod(std::string name);
    std::string ToString();

    Object Call(Interpreter *interpreter, std::vector<Object> arguments) override;

    int Arity() override;
};

#endif // LOXCLASS_H