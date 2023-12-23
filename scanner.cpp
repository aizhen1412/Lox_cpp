/*
 * scanner.cpp
 * This file implements the Scanner class defined in scanner.h.
 * The Scanner class is used to scan the source code and generate a list of tokens.
 * It includes methods for scanning individual tokens, checking the next character in the source code, and checking if the end of the source code has been reached.
 */

#include "scanner.h"
#include "token_type_functions.h"
#include "error.h"

Scanner::Scanner(std::string source) : source(source) {}

std::vector<Token> Scanner::ScanTokens()
{
    while (!IsAtEnd())
    {
        // We are at the beginning of the next lexeme.
        start = current;
        ScanToken();
    }

    return tokens;
}

void Scanner::ScanToken()
{
    char c = Advance();
    switch (c)
    {
    case '(':
        AddToken(LEFT_PAREN);
        break;
    case ')':
        AddToken(RIGHT_PAREN);
        break;
    case '{':
        AddToken(LEFT_BRACE);
        break;
    case '}':
        AddToken(RIGHT_BRACE);
        break;
    case ',':
        AddToken(COMMA);
        break;
    case '.':
        AddToken(DOT);
        break;
    case '-':
        AddToken(MINUS);
        break;
    case '+':
        AddToken(PLUS);
        break;
    case ';':
        AddToken(SEMICOLON);
        break;
    case '*':
        AddToken(STAR);
        break;
    case '!':
        AddToken(Match('=') ? BANG_EQUAL : BANG);
        break;
    case '=':
        AddToken(Match('=') ? EQUAL_EQUAL : EQUAL);
        break;
    case '<':
        AddToken(Match('=') ? LESS_EQUAL : LESS);
        break;
    case '>':
        AddToken(Match('=') ? GREATER_EQUAL : GREATER);
        break;
    case '/':
        if (Match('/'))
        {
            while (Peek() != '\n' && !IsAtEnd())
                Advance();
        }
        else
        {
            AddToken(SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;
    case '\n':
        line++;
        break;
    case '"':
        String();
        break;
    default:
        if (IsDigit(c))
        {
            Number();
        }
        else if (IsAlpha(c))
        {
            Identifier();
        }
        else
        {
            Error::ReportError(line, "Unexpected character.");
        }
        break;
    }
}
/*
 * Returns the current character without consuming it.
 * If the current character index is beyond the end of the source string, it returns a null character.
 * Otherwise, it returns the current character.
 */
char Scanner::Peek()
{
    if (IsAtEnd())
        return '\0';
    return source[current];
}

char Scanner::PeekNext()
{
    if (current + 1 >= source.length())
        return '\0';
    return source[current + 1];
}

char Scanner::Advance()
{
    current++;
    return source[current - 1];
}

void Scanner::AddToken(TokenType type)
{
    AddToken(type, nullptr);
}

void Scanner::AddToken(TokenType type, Object literal)
{
    std::string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
}

void Scanner::Number()
{
    while (IsDigit(Peek()))
        Advance();

    // Look for a fractional part.
    if (Peek() == '.' && IsDigit(PeekNext()))
    {
        // Consume the "."
        Advance();

        while (IsDigit(Peek()))
            Advance();
    }

    AddToken(NUMBER, std::stod(source.substr(start, current - start)));
}

void Scanner::Identifier()
{
    while (IsAlphaNumeric(Peek()))
        Advance();

    std::string text = source.substr(start, current - start);

    TokenType type;
    auto it = keywords.find(text);

    if (it != keywords.end())
    {
        type = it->second;
    }
    else
    {
        type = TokenType::IDENTIFIER;
    }

    AddToken(type);
}

void Scanner::String()
{
    while (Peek() != '"' && !IsAtEnd())
    {
        if (Peek() == '\n')
            line++;
        Advance();
    }

    if (IsAtEnd())
    {
        Error::ReportError(line, "Unterminated string.");
        return;
    }

    // The closing ".
    Advance();

    // Trim the surrounding quotes.
    std::string value = source.substr(start + 1, current - start - 2);
    AddToken(STRING, value);
}

bool Scanner::IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Scanner::Match(const char expected)
{
    if (IsAtEnd())
        return false;
    if (source[current] != expected)
        return false;

    current++;
    return true;
}

bool Scanner::IsAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Scanner::IsAlphaNumeric(char c)
{
    return IsAlpha(c) || IsDigit(c);
}

bool Scanner::IsAtEnd()
{
    return current >= source.length();
}