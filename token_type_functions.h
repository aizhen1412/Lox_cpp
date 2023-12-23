/*
 * token_type_functions.h
 * This file declares the TokenType2String function, which converts a TokenType enum value to a string.
 * This function is used for debugging and error reporting purposes.
 */
#ifndef TOKEN_TYPE_FUNCTIONS_H
#define TOKEN_TYPE_FUNCTIONS_H

#include <string>
#include "token_type_enum.h"

std::string TokenType2String(TokenType tokenType);

#endif // TOKEN_TYPE_FUNCTIONS_H