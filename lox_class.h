#ifndef LOXCLASS_H
#define LOXCLASS_H

#include <string>
#include <unordered_map>
#include "visit_call_expr.h"
#include "lox_function.h"

class LoxFunction;

class LoxClass : public LoxCallable
{
public:
    LoxClass(){};
    LoxClass(std::string name, LoxClass *superclass, std::unordered_map<std::string, LoxFunction *> methods);
    ~LoxClass();

    LoxFunction *FindMethod(std::string name); // 查找
    std::string ToString();
    Object Call(Interpreter *interpreter, std::vector<Object> arguments) override;
    int Arity() override; // 返回参数个数
    std::string Get_name();

private:
    std::string name;
    LoxClass *superclass;
    std::unordered_map<std::string, LoxFunction *> methods;
};

#endif // LOXCLASS_H