#include "lox_class.h"
#include "lox_instance.h"
#include "interpreter.h"

LoxClass::LoxClass(std::string name, LoxClass *superclass, std::unordered_map<std::string, LoxFunction *> methods) : name(name), superclass(superclass), methods(methods) {}
LoxClass::~LoxClass()
{
    for (auto it = methods.begin(); it != methods.end(); it++)
        delete it->second;
}
std::string LoxClass::ToString()
{
    return name;
}
LoxFunction *LoxClass::FindMethod(std::string name)
{
    auto it = methods.find(name);
    if (it != methods.end())
    {
        return it->second;
    }
    if (superclass != nullptr)
    {
        return superclass->FindMethod(name);
    }
    return nullptr;
}
Object LoxClass::Call(Interpreter *interpreter, std::vector<Object> arguments)
{
    LoxInstance *instance = new LoxInstance(this); // instance在environment的析构函数中会被delete
    LoxFunction *initializer = FindMethod("init");
    if (initializer != nullptr)
    {
        LoxFunction *temp = initializer->Bind(instance);
        temp->Call(interpreter, arguments);
        delete temp;
    }
    return instance;
}

int LoxClass::Arity()
{
    LoxFunction *initializer = FindMethod("init");
    if (initializer == nullptr)
        return 0;
    return initializer->Arity();
}
std::string LoxClass::Get_name()
{
    return name;
}
