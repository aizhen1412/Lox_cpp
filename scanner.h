/*
 * scanner.h
 * This file defines the Scanner class, which is used to scan the source code and generate a list of tokens.
 * The Scanner class includes methods for scanning individual tokens, checking the next character in the source code, and checking if the end of the source code has been reached.
 */
#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "token.h"
#include "token_type_enum.h"

class Scanner
{
public:
    Scanner(std::string source);
    std::vector<Token> ScanTokens();

private:
    const std::string source;
    std::vector<Token> tokens;
    unsigned start = 0;
    unsigned current = 0;
    unsigned line = 1;

    std::unordered_map<std::string, TokenType> keywords = {
        {"and", TokenType::AND},
        {"class", TokenType::CLASS},
        {"else", TokenType::ELSE},
        {"false", TokenType::FALSE},
        {"for", TokenType::FOR},
        {"fun", TokenType::FUN},
        {"if", TokenType::IF},
        {"nil", TokenType::NIL},
        {"or", TokenType::OR},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super", TokenType::SUPER},
        {"this", TokenType::THIS},
        {"true", TokenType::TRUE},
        {"var", TokenType::VAR},
        {"while", TokenType::WHILE}};

    void ScanToken(); // scan the source code and generate a list of tokens
    char Peek();      // check the current character
    char PeekNext();  // check the next character
    char Advance();   // advance the current character
    void AddToken(TokenType type);
    void AddToken(TokenType type, Object literal);
    void Number();
    void Identifier();
    void String();
    bool IsDigit(char c);
    bool Match(const char expected); // check if the current character matches the expected character
    bool IsAlpha(char c);
    bool IsAlphaNumeric(char c);
    bool IsAtEnd();
};
#endif // SCANNER_H