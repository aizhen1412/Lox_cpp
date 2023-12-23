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
Environment::~Environment()
{
    delete enclosing;
    for (auto it = values.begin(); it != values.end(); it++)
    {
        std::visit([](auto &&arg)
                   { using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_pointer_v<T>)
                    {
                        delete arg;
                    } },
                   it->second);
    }
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
    return Ancestor(distance)->values[name];
}
void Environment::AssignAt(int distance, Token name, Object value)
{
    Ancestor(distance)->values[name.lexeme] = value;
}
Environment *Environment::Ancestor(int distance)
{
    Environment *environment = this;
    for (int i = 0; i < distance; i++)
    {
        environment = environment->enclosing;
    }

    return environment;
}
Environment *Environment::Get_enclosing()
{
    return enclosing;
}
Environment *Environment::Set_enclosing(Environment *enclosing)
{
    this->enclosing = enclosing;
    return enclosing;
}