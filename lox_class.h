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

    LoxClass(std::string name, LoxClass *superclass, std::unordered_map<std::string, LoxFunction *> methods)
    {
        this->superclass = superclass;
        this->name = name;
        this->methods = methods;
    }
    LoxFunction *findMethod(std::string name)
    {
        if (auto it = methods.find(name) != methods.end())
        {
            return methods[name];
        }
        if (superclass != nullptr)
        {
            return superclass->findMethod(name);
        }
        return nullptr;
    }
    std::string toString();

    Object call(Interpreter *interpreter, std::vector<Object> arguments) override;

    int arity() override;
};

#endif // LOXCLASS_H