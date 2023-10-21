#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>
#include <unordered_map>
#include "token.h"

class Environment
{
public:
    Environment();
    Environment(Environment *enclosing);
    Object get(Token name);
    void assign(const Token &name, Object value);
    void define(const std::string &name, Object value);

private:
    Environment *enclosing;
    std::unordered_map<std::string, Object> values;
};

#endif // ENVIRONMENT_H