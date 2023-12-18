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
        // std::cout << "get " << name.lexeme << std::endl;
        // if (std::holds_alternative<LoxClass *>(values[name.lexeme]) == true)
        // {
        //     std::cout << " class" << std::endl;
        // }
        // if (std::holds_alternative<bool>(values[name.lexeme]))
        // {
        //     std::cout << "test" << std::get<bool>(values[name.lexeme]) << std::endl;
        // }
        // std::cout << values[name.lexeme] << std::endl;
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
    // if (std::holds_alternative<double>(value) == true)
    // {
    //     std::cout << "define " << name << "=" << std::get<double>(value) << std::endl;
    // }
}
Object Environment::getAt(int distance, std::string name)
{
    //  std::cout << "name " << name << std::endl;
    return ancestor(distance)->values[name];
}
void Environment::assignAt(int distance, Token name, Object value)
{
    ancestor(distance)->values[name.lexeme] = value;
}
Environment *Environment::ancestor(int distance)
{
    // std::cout << distance << std::endl;
    Environment *environment = this;
    //  std::cout << "environment " << environment << std::endl;
    for (int i = 0; i < distance; i++)
    {
        environment = environment->enclosing;
    }
    // 打印values
    // for (auto it = environment->values.begin(); it != environment->values.end(); it++)
    // {
    //     std::cout << it->first << " " << it->second << std::endl;
    // }
    // 判断environment->values["n"]是否为double
    // if (std::holds_alternative<double>(environment->values["n"]) == true)
    // {
    //     std::cout << "n is double" << std::endl;
    //     std::cout << std::get<double>(environment->values["n"]) << std::endl;
    // }

    //  std::cout << environment->values["n"] << std::endl;
    return environment;
}