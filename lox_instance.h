#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H
#include <string>
#include <unordered_map>
#include "lox_class.h" // 包含 LoxClass 类的头文件

class LoxInstance
{
public:
    LoxClass *klass;

    LoxInstance(LoxClass *klass);
    Object get(Token name);

    void set(Token name, Object value)
    {
        fields[name.lexeme] = value;
    }

    std::string toString();

    std::unordered_map<std::string, Object> fields;
};

#endif
