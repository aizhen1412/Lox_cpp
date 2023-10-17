#include <sstream>
#include "ast_printer.h"

std::string ToStringVisitor::operator()(const double &value) const
{
    return std::to_string(value);
}
std::string ToStringVisitor::operator()(const std::string &value) const
{
    return value;
}
std::string ToStringVisitor::operator()(std::nullptr_t) const
{
    return "nil";
}

Object AstPrinter::print(Expr &expr)
{
    return expr.Accept(*this);
}
Object AstPrinter::VisitBinary(Binary &expr)
{
    return parenthesize(expr.op.lexeme, expr.left, expr.right);
}
Object AstPrinter::VisitGrouping(Grouping &expr)
{
    return parenthesize("group", expr.expression);
}

Object AstPrinter::VisitUnary(Unary &expr)
{
    return parenthesize(expr.op.lexeme, expr.right);
}
Object AstPrinter::VisitLiteral(Literal &expr)
{
    std::string result = std::visit(ToStringVisitor(), expr.value);
    return result;
}

Object AstPrinter::VisitCall(Call &Expr)
{
    return "test";
};
Object AstPrinter::VisitAssignExpr(Assign &Expr)
{
    return "test";
};
Object AstPrinter::VisitGet(Get &Expr)
{
    return "test";
};
Object AstPrinter::VisitLogical(Logical &Expr)
{
    return "test";
};
Object AstPrinter::VisitSet(Set &Expr)
{
    return "test";
};
Object AstPrinter::VisitSuper(Super &Expr)
{
    return "test";
};
Object AstPrinter::VisitThis(This &Expr)
{
    return "test";
};
Object AstPrinter::VisitVariable(Variable &Expr)
{
    return "test";
};
Object AstPrinter::visitBlockStmt(Block &stmt)
{
    return "test";
};
Object AstPrinter::visitClassStmt(Class &stmt)
{
    return "test";
};
Object AstPrinter::visitExpressionStmt(Expression &stmt)
{
    return "test";
};
Object AstPrinter::visitFunctionStmt(Function &stmt)
{
    return "test";
};
Object AstPrinter::visitIfStmt(If &stmt)
{
    return "test";
};
Object AstPrinter::visitPrintStmt(Print &stmt)
{
    return "test";
};
Object AstPrinter::visitReturnStmt(Return &stmt)
{
    return "test";
};
Object AstPrinter::visitVarStmt(Var &stmt)
{
    return "test";
};
Object AstPrinter::visitWhileStmt(While &stmt)
{
    return "test";
};

template <typename T, typename... Args>
std::string AstPrinter::parenthesize(const T &name, Args... expr)
{
    std::stringstream ss;
    ss << "(" << name;

    ((ss << " " << std::get<std::string>(expr->Accept(*this))), ...);

    ss << ")";

    return ss.str();
}
