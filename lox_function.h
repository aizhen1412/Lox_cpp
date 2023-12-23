#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include "visit_call_expr.h"
#include "environment.h"

class Interpreter;

class LoxFunction : public LoxCallable
{
public:
    LoxFunction(){};
    LoxFunction(Function declaration, Environment *closure, bool isInitializer);
    ~LoxFunction();
    LoxFunction *Bind(LoxInstance *instance);
    Object Call(Interpreter *interpreter, std::vector<Object> arguments);
    int Arity();

private:
    Function declaration;
    Environment *closure;
    bool is_initializer;

    std::string ToString();
};
#endif // LOX_FUNCTION_H