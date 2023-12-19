#ifndef LOXINSTANCE_H
#define LOXINSTANCE_H

#include <string>
#include <unordered_map>
#include "lox_class.h" // 包含 LoxClass 类的头文件

class LoxInstance
{
public:
    LoxInstance(LoxClass *klass);
    void Set(Token name, Object value);
    Object Get(Token name);
    std::string ToString();

    LoxClass *klass;
    std::unordered_map<std::string, Object> fields;
};

#endif // LOXINSTANCE_H
