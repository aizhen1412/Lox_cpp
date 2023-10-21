#include <iostream>
#include "environment.h"
#include "error.h"

Environment::Environment()
{
    this->enclosing = nullptr;
}
Environment::Environment(Environment *enclosing)
{
    this->enclosing = enclosing;
}

Object Environment::get(Token name)
{

    if (values.find(name.lexeme) != values.end())
    {
        //  std::cout << "get " << name.lexeme << std::endl;
        // std::cout << std::get<LoxCallable>(values[name.lexeme]) << std::endl;
        return values[name.lexeme];
    }
    if (enclosing != nullptr)
        return enclosing->get(name);
    throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
void Environment::assign(const Token &name, Object value)
{
    auto it = values.find(name.lexeme);
    if (it != values.end())
    {
        it->second = value;
        return;
    }
    if (enclosing != nullptr)
    {
        enclosing->assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::define(const std::string &name, Object value)
{
    values[name] = value;
    if (std::holds_alternative<bool>(value) == true)
    {
        std::cout << "define " << name << std::endl;
        std::cout << std::get<bool>(value) << std::endl;
    }
}
