#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <memory>
#include "ast_printer.h"
#include "expr.h"
#include "environment.h"
#include "lox_function.h"
#include "error.h"
#include "lox_class.h"

class Interpreter : public Visitor // 后面换成visitor
{
public:
    //  Interpreter() = default;
    ~Interpreter();
    void Interpret(std::vector<Stmt *> statements);                              // 解释器入口
    void ExecuteBlock(std::vector<Stmt *> statements, Environment *environment); // 执行块
    void Resolve(Expr *expr, int depth);                                         // 解析变量

private:
    Environment *globals = new Environment(); // 全局环境
    Environment *environment = globals;       // 当前环境
    std::unordered_map<Expr *, int> locals;   // 用于存储变量的深度

    Object VisitGetExpr(Get &Expr) override;           // 从类中获取属性
    Object VisitSuperExpr(Super &Expr) override;       // 从父类中获取属性
    Object VisitLiteralExpr(Literal &expr) override;   // 访问字面量
    Object VisitLogicalExpr(Logical &expr) override;   // 访问逻辑表达式
    Object VisitSetExpr(Set &Expr) override;           // 设置属性
    Object VisitThisExpr(This &expr);                  // 访问this表达式
    Object VisitUnaryExpr(Unary &expr) override;       // 访问一元表达式
    Object VisitVariableExpr(Variable *expr) override; // 访问变量表达式
    Object VisitGroupingExpr(Grouping &expr) override; // 访问分组表达式
    Object VisitBinaryExpr(Binary &expr) override;     // 访问二元表达式
    Object VisitCallExpr(Call &expr);                  // 访问函数调用表达式
    Object LookUpVariable(Token name, Expr *expr);     // 查找变量

    void CheckNumberOperand(Token op, Object operand);             // 检查操作数是否为数字
    void CheckNumberOperands(Token op, Object left, Object right); // 检查操作数是否为数字
    bool IsTruthy(Object object);                                  // 判断是否为真值
    bool IsEqual(Object a, Object b);                              // 判断两个对象是否相等
    Object Evaluate(Expr *expr);                                   // 计算表达式
    void Execute(Stmt *stmt);                                      // 执行语句

    Object VisitBlockStmt(Block &stmt) override;           // 访问块
    Object VisitClassStmt(Class &stmt) override;           // 访问类
    Object VisitExpressionStmt(Expression &stmt) override; // 访问表达式
    Object VisitFunctionStmt(Function &stmt);              // 访问函数
    Object VisitIfStmt(If &stmt);                          // 访问if
    Object VisitPrintStmt(Print &stmt) override;           // 访问print
    Object VisitReturnStmt(Return &stmt);                  // 访问return
    Object VisitVarStmt(Var &stmt) override;               // 访问var
    Object VisitWhileStmt(While &stmt) override;           // 访问while
    Object VisitAssignExpr(Assign &expr);                  //

    std::string Stringify(Object object); // 将对象转换为字符串
};

#endif // INTERPRETER_H