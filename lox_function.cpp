#include "lox_function.h"
#include "return_method.h"
#include "interpreter.h"

LoxFunction::LoxFunction(Function declaration, Environment *closure, bool isInitializer) : declaration(declaration), closure(closure), is_initializer(isInitializer) {}
LoxFunction::~LoxFunction()
{
    delete closure;
}
LoxFunction *LoxFunction::Bind(LoxInstance *instance)
{
    Environment *environment = new Environment(closure); // environment在loxfuction的析构函数中会被delete
    environment->Define("this", instance);
    return new LoxFunction(declaration, environment, is_initializer); // loxfunction在调用的函数中会被delete
}
Object LoxFunction::Call(Interpreter *interpreter, std::vector<Object> arguments)
{
    Environment *environment = new Environment(closure);
    for (std::vector<Token>::size_type i = 0; i < declaration.params.size(); i++)
    {
        environment->Define(declaration.params[i].lexeme, arguments[i]);
    }
    try
    {
        interpreter->ExecuteBlock(declaration.body, environment);
        environment->Set_enclosing(nullptr);
        delete environment;
    }
    catch (const Return_method &returnValue)
    {
        if (is_initializer)
            return closure->GetAt(0, "this");
        return returnValue.value;
    }
    if (is_initializer)
        return closure->GetAt(0, "this");
    return nullptr;
}
int LoxFunction::Arity()
{
    return declaration.params.size();
}
std::string LoxFunction::ToString()
{
    return "<fn " + declaration.name.lexeme + ">";
}
