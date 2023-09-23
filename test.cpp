// #include <vector>
// #include <stdexcept>
// #include "token.h"
// #include "error.h"
// #include "expr.h"
// #include "scanner.h"
// #include "ast_printer.h"

// class Parser
// {
// public:
//     Parser(std::vector<Token> tokens)
//     {
//         this->tokens = tokens;
//     }
//     Expr *Parse() // 解析表达式
//     {
//         try
//         {
//             return Expression();
//         }
//         catch (ParseError error)
//         {
//             return nullptr;
//         }
//     }

// private:
//     int current = 0;
//     std::vector<Token> tokens;

//     class ParseError
//     {
//     };
//     Expr *Expression()
//     {
//         return Equality(); // 等式
//     }
//     Expr *Equality()
//     {
//         Expr *expr = Comparison();

//         while (Match(BANG_EQUAL, EQUAL_EQUAL))
//         {
//             Token op = Previous(); // 操作符
//             Expr *right = Comparison();
//             expr = new Binary(expr, op, right);
//         }

//         return expr;
//     }
//     Expr *Comparison()
//     {
//         Expr *expr = Term();

//         while (Match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
//         {
//             Token op = Previous();
//             Expr *right = Term();
//             expr = new Binary(expr, op, right);
//         }

//         return expr;
//     }
//     Expr *Term()
//     {
//         Expr *expr = Factor();

//         while (Match(MINUS, PLUS))
//         {
//             Token op = Previous();
//             Expr *right = Factor();
//             expr = new Binary(expr, op, right);
//         }

//         return expr;
//     }
//     Expr *Factor()
//     {
//         Expr *expr = Unary();

//         while (Match(SLASH, STAR))
//         {
//             Token op = Previous();
//             Expr *right = Unary();
//             expr = new Binary(expr, op, right);
//         }

//         return expr;
//     }
//     Expr *Unary()
//     {
//         if (Match(BANG, MINUS))
//         {
//             Token op = Previous();
//             Expr *right = Unary();
//             return new class Unary(op, right);
//         }

//         return Primary();
//     }
//     Expr *Primary()
//     {
//         if (Match(FALSE))
//             return new Literal(false);
//         if (Match(TRUE))
//             return new Literal(true);
//         if (Match(NIL))
//             return new Literal(nullptr);

//         if (Match(NUMBER, STRING))
//         {
//             return new Literal(Previous().literal);
//         }

//         if (Match(LEFT_PAREN))
//         {
//             Expr *expr = Expression();
//             Consume(RIGHT_PAREN, "Expect ')' after expression.");
//             return new Grouping(expr);
//         }
//         throw error(Peek(), "Expect expression.");
//         // exit(65);
//     }

//     template <typename... Args>
//     bool Match(Args... types)
//     {
//         TokenType typesArray[] = {types...};

//         for (TokenType type : typesArray)
//         {
//             if (Check(type))
//             {
//                 Advance();
//                 return true;
//             }
//         }
//         return false;
//     }
//     Token Consume(TokenType type, std::string message)
//     {
//         if (Check(type))
//             return Advance();

//         throw error(Peek(), message);
//         // exit(65);
//     }

//     ParseError error(Token token, std::string message)
//     {
//         Error::ErrorFind(token, message);
//         return ParseError();
//     }
//     void Synchronize()
//     {
//         Advance();

//         while (!IsAtEnd())
//         {
//             if (Previous().type == SEMICOLON)
//                 return;

//             switch (Peek().type)
//             {
//             case CLASS:
//             case FUN:
//             case VAR:
//             case FOR:
//             case IF:
//             case WHILE:
//             case PRINT:
//             case RETURN:
//                 return;
//             }

//             Advance();
//         }
//     }
//     bool Check(TokenType type)
//     {
//         if (IsAtEnd())
//             return false;
//         return Peek().type == type;
//     }
//     Token Advance()
//     {
//         if (!IsAtEnd())
//             current++;
//         return Previous();
//     }

//     bool IsAtEnd()
//     {
//         return Peek().type == END_OF_FILE;
//     }

//     Token Peek()
//     {
//         return tokens[current];
//     }

//     Token Previous()
//     {
//         return tokens[current - 1];
//     }
// };
// int main()
// {
//     std::string line;
//     while (true)
//     {
//         std::cout << "> ";
//         std::getline(std::cin, line);
//         if (std::cin.eof() || line.empty())
//         {
//             break;
//         }
//         line = line + "EOF";                              // 添加结束符
//         Scanner scanner(line);                            // 词法分析
//         std::vector<Token> tokens = scanner.ScanTokens(); // 存储词法单元

//         Parser parser(tokens);
//         Expr *expression = parser.Parse();
//         if (expression == nullptr)
//         {
//             continue;
//         }

//         // Stop if there was a syntax error.-
//         if (had_error)
//         { // exit(65);
//             had_error = false;
//             continue;
//         }
//         AstPrinter astPrinter;
//         std::cout << (astPrinter.print(*expression)) << std::endl;
//     }

//     return 0;
// }