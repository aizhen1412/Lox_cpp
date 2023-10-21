#ifndef LOX_FUNCTION_H
#define LOX_FUNCTION_H

#include "visit_call_expr.h"
#include "Interpreter.h"

class LoxFunction : public LoxCallable
{
public:
    Function declaration;
    Environment *closure;
    LoxFunction();
    LoxFunction(Function &declaration, Environment *closure);
    Object call(Interpreter *interpreter, std::vector<Object> arguments);
    int arity();
    std::string toString();
};
#endif // LOX_FUNCTION_H