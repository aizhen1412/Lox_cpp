#ifndef RETURN_METHOD_H
#define RETURN_METHOD_H

#include <iostream>
#include <stdExcept>
#include "error.h"

class Return_method : public std::exception
{
public:
    Return_method(Object value)
    {
        this->value = value;
    }

    Object value;
};
#endif // RETURN_METHOD_H