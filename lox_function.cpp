#include "lox_function.h"
#include "return_method.h"
#include "interpreter.h"

LoxFunction::LoxFunction() = default;
// LoxFunction::LoxFunction(Function &declaration, Environment *closure)
// {
//     this->closure = closure;
//     this->declaration = declaration;
// }
Object LoxFunction::call(Interpreter *interpreter, std::vector<Object> arguments)
{
    Environment *environment = new Environment(closure);
    for (int i = 0; i < declaration.params.size(); i++)
    {
        environment->define(declaration.params[i].lexeme, arguments[i]);
    }
    try
    {
        interpreter->executeBlock(declaration.body, environment);
    }
    catch (const Return_method &returnValue)
    {
        //   std::cout << "return " << interpreter->previous << std::endl;
        // std::cout << "in catch" << std::endl;
        // interpreter->environment = interpreter->previous;
        // interpreter->count++;
        // std::cout << "call recover " << std::endl;
        //   interpreter->test();
        if (isInitializer)
            return closure->getAt(0, "this");
        return returnValue.value;
    }
    if (isInitializer)
        return closure->getAt(0, "this");
    return nullptr;
}
int LoxFunction::arity()
{
    return declaration.params.size();
}
std::string LoxFunction::toString()
{
    return "<fn " + declaration.name.lexeme + ">";
}
