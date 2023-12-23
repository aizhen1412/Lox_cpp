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
    ~Environment();
    Object get(Token name);
    void Assign(const Token &name, Object value);
    void Define(const std::string &name, Object value);
    Object GetAt(int distance, std::string name);
    void AssignAt(int distance, Token name, Object value);
    Environment *Ancestor(int distance);
    Environment *Get_enclosing();
    Environment *Set_enclosing(Environment *enclosing);

private:
    Environment *enclosing;                         // 当前环境的闭包
    std::unordered_map<std::string, Object> values; // 存储变量的值
};

#endif // ENVIRONMENT_H