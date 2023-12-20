#include "lox_function.h"
#include "return_method.h"
#include "interpreter.h"

LoxFunction::LoxFunction() = default;
LoxFunction::LoxFunction(Function declaration, Environment *closure, bool isInitializer) : declaration(declaration), closure(closure), is_initializer(isInitializer) {}

LoxFunction *LoxFunction::Bind(LoxInstance *instance)
{
    Environment *environment = new Environment(closure);
    environment->Define("this", instance);
    return new LoxFunction(declaration, environment, is_initializer);
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
