#include "lox_class.h"
#include "lox_instance.h"
#include "interpreter.h"

// LoxClass::LoxClass(std::string name, std::unordered_map<std::string, LoxFunction *> methods) : name(name), methods(methods)
// {
//     // 构造函数的实现
// }

std::string LoxClass::toString()
{
    return name;
}

Object LoxClass::call(Interpreter *interpreter, std::vector<Object> arguments)
{
    // std::cout << "LoxClass::call" << std::endl;
    LoxInstance *instance = new LoxInstance(this);
    LoxFunction *initializer = findMethod("init");
    if (initializer != nullptr)
    {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    return instance;
}

int LoxClass::arity()
{
    LoxFunction *initializer = findMethod("init");
    if (initializer == nullptr)
        return 0;
    return initializer->arity();
}
