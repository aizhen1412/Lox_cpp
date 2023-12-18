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
    Object Get(Token name);

    void Set(Token name, Object value);
    std::string ToString();

    std::unordered_map<std::string, Object> fields;
};

#endif
