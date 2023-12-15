#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include "visit_call_expr.h"
#include "environment.h"

class Interpreter;

class LoxFunction : public LoxCallable
{
public:
    Function declaration;
    Environment *closure;

    bool isInitializer;

    LoxFunction(Function declaration, Environment *closure, bool isInitializer)
    {
        this->isInitializer = isInitializer;
        this->closure = closure;
        this->declaration = declaration;
    }
    LoxFunction();
    //  LoxFunction(Function &declaration, Environment *closure);
    Object call(Interpreter *interpreter, std::vector<Object> arguments);
    int arity();
    std::string toString();

    LoxFunction *bind(LoxInstance *instance)
    {
        Environment *environment = new Environment(closure);
        environment->define("this", instance);
        return new LoxFunction(declaration, environment, isInitializer);
    }
};
#endif // LOX_FUNCTION_H