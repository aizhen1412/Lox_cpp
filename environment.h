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
    void Assign(const Token &name, Object value);
    void Define(const std::string &name, Object value);
    Object GetAt(int distance, std::string name);
    void AssignAt(int distance, Token name, Object value);
    Environment *Ancestor(int distance);

    Environment *enclosing;
    std::unordered_map<std::string, Object> values;
};

#endif // ENVIRONMENT_H