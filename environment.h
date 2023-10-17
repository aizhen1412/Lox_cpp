#include <string>
#include <unordered_map>
#include "token.h"
#include "error.h"

class Environment
{
public:
    Environment()
    {
        this->enclosing = nullptr;
    }
    Environment(Environment *enclosing)
    {
        this->enclosing = enclosing;
    }
    Environment *enclosing;
    std::unordered_map<std::string, Object> values;
    Object get(Token name)
    {
        if (values.find(name.lexeme) != values.end())
        {
            return values[name.lexeme];
        }
        if (enclosing != nullptr)
            return enclosing->get(name);
        throw new RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
    }
    void assign(const Token &name, Object value)
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

    void define(const std::string &name, Object value)
    {
        values[name] = value;
    }
};
