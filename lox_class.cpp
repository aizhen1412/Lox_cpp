/*
 * lox_class.cpp
 * This file implements the LoxClass class defined in lox_class.h.
 * The LoxClass class represents a user-defined class in the Lox language.
 *
 * The constructor initializes the class with a name, a superclass, and a map of methods.
 *
 * The destructor deletes the methods.
 *
 * The FindMethod method returns the method with the given name, or null if the method is not found. If the method is not found in the class, it looks for the method in the superclass.
 *
 * The Call method creates a new instance of the class and calls the initializer method, if it exists. The initializer method is bound to the instance before it is called.
 *
 * The Arity method returns the number of parameters the initializer method expects, or zero if the initializer method does not exist.
 *
 * The ToString method returns the name of the class.
 *
 * The Get_name method also returns the name of the class.
 *
 * Author: Your Name
 * Date: Current Date
 */
#include "lox_class.h"
#include "lox_instance.h"
#include "interpreter.h"

LoxClass::LoxClass(std::string name, LoxClass *superclass, std::unordered_map<std::string, LoxFunction *> methods) : name(name), superclass(superclass), methods(methods) {}
LoxClass::~LoxClass()
{
    for (auto it = methods.begin(); it != methods.end(); it++)
        delete it->second;
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
std::string LoxClass::ToString()
{
    return name;
}