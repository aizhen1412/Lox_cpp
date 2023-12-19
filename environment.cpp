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
        return values[name.lexeme];
    }
    if (enclosing != nullptr)
        return enclosing->get(name);
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
void Environment::Assign(const Token &name, Object value)
{
    auto it = values.find(name.lexeme);
    if (it != values.end())
    {
        it->second = value;
        return;
    }
    if (enclosing != nullptr)
    {
        enclosing->Assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::Define(const std::string &name, Object value)
{
    values[name] = value;
}
Object Environment::GetAt(int distance, std::string name)
{
    //  std::cout << "name " << name << std::endl;
    return Ancestor(distance)->values[name];
}
void Environment::AssignAt(int distance, Token name, Object value)
{
    Ancestor(distance)->values[name.lexeme] = value;
}
Environment *Environment::Ancestor(int distance)
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