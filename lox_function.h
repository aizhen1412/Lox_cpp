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

    bool is_initializer;
    LoxFunction();
    LoxFunction(Function declaration, Environment *closure, bool isInitializer);

    Object Call(Interpreter *interpreter, std::vector<Object> arguments);
    int Arity();
    std::string ToString();

    LoxFunction *Bind(LoxInstance *instance);
};
#endif // LOX_FUNCTION_H