#include "scanner.h"
#include "token_type_functions.h"
#include "error.h"

Scanner::Scanner(std::string source) : source(source)
{
    // 构造函数体为空，因为成员变量已经在初始化列表中初始化
}

std::vector<Token> Scanner::ScanTokens() // 扫描源代码
{
    while (!IsAtEnd()) // 不是到达末尾
    {
        // We are at the beginning of the next lexeme.
        start = current; // 记录当前词法单元的起始位置
        ScanToken();
    }

    return tokens;
}

void Scanner::ScanToken() // 扫描词法单元
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
            while (Peek() != '\n' && !IsAtEnd()) // 读取注释
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
    case '\n': // 处理换行符
        line++;
        break;
    case '"': // 处理字符串
        String();
        break;
    default: // 处理数字和标识符
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

char Scanner::Peek() // 查看下一个字符
{
    if (IsAtEnd())
        return '\0';
    return source[current]; // 返回当前词法单元的下一个字符
}

char Scanner::PeekNext() // 查看下下一个字符
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

void Scanner::AddToken(TokenType type) // 添加词法单元
{
    AddToken(type, nullptr);
}

void Scanner::AddToken(TokenType type, Object literal) // 添加词法单元
{
    std::string text = source.substr(start, current - start); // 从源代码中截取出当前词法单元的字符串
    tokens.push_back(Token(type, text, literal, line));       // 将当前词法单元添加到词法单元列表中
}

void Scanner::Number()
{
    while (IsDigit(Peek())) // 整数
        Advance();

    // Look for a fractional part.
    if (Peek() == '.' && IsDigit(PeekNext())) // 浮点数
    {
        // Consume the "."
        Advance();

        while (IsDigit(Peek()))
            Advance();
    }

    AddToken(NUMBER, std::stod(source.substr(start, current - start))); // 将字符串转换为double类型加入词法单元列表中
}

void Scanner::Identifier() // 处理标识符
{
    while (IsAlphaNumeric(Peek()))
        Advance();

    std::string text = source.substr(start, current - start); // 截出字符串

    TokenType type; // 标识符的类型
    auto it = keywords.find(text);

    if (it != keywords.end())
    {
        type = it->second;
        // 处理已找到的情况
    }
    else
    {
        type = TokenType::IDENTIFIER;
        // 处理未找到的情况默认为IDENTIFIER
    }

    AddToken(type);
}

void Scanner::String() // 处理字符串
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

bool Scanner::IsDigit(char c) // 判断是否为数字
{
    return c >= '0' && c <= '9';
}

bool Scanner::Match(const char expected) // 判断当前词法单元是否匹配
{
    if (IsAtEnd())
        return false;
    if (source[current] != expected)
        return false;

    current++;
    return true;
}

bool Scanner::IsAlpha(char c) // 判断是否为字母
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Scanner::IsAlphaNumeric(char c) // 判断是否为字母或数字
{
    return IsAlpha(c) || IsDigit(c);
}

bool Scanner::IsAtEnd() // 判断是否到达末尾
{
    return current >= source.length();
}