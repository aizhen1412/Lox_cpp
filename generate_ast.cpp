// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <vector>
// #include <string>
// #include <algorithm>

// void defineAst(
//     const std::string &outputDir,
//     const std::string &baseName,
//     const std::vector<std::string> &types)
// {
//     std::ofstream outputFile(outputDir + "/" + "expr.h");

//     outputFile << "#ifndef EXPR_H" << std::endl
//                << "#define EXPR_H" << std::endl
//                << std::endl;
//     outputFile << "#include <vector>" << std::endl;
//     outputFile << "#include \"token.h\"" << std::endl
//                << std::endl;
//     outputFile << "class Visitor;" << std::endl;

//     for (const std::string &type : types) // Define all the classes
//     {
//         std::string typeName = type.substr(0, type.find(' '));
//         outputFile << "class " << typeName << ";" << std::endl;
//     }
//     // Define the base class
//     outputFile << std::endl
//                << "class " << baseName << std::endl
//                << "{" << std::endl;
//     outputFile << "public:" << std::endl;
//     outputFile << "    virtual ~" << baseName << "() {}" << std::endl;
//     outputFile << "    virtual std::string Accept(Visitor& visitor) = 0;" << std::endl;
//     outputFile << "};" << std::endl
//                << std::endl;

//     // Define visitor interface
//     outputFile << "class Visitor" << std::endl
//                << "{" << std::endl;
//     outputFile << "public:" << std::endl;

//     for (const std::string &type : types)
//     {
//         std::string typeName = type.substr(0, type.find(':'));
//         outputFile << "    virtual std::string Visit" << typeName << "("
//                    << typeName << "& " << baseName << ") = 0;" << std::endl;
//     }

//     outputFile << "};" << std::endl
//                << std::endl;

//     // Define AST classes
//     for (const std::string &type : types)
//     {
//         std::string typeName = type.substr(0, type.find(' ')); // 冒号前面的部分
//         std::string fields = type.substr(type.find(':') + 1);  // 冒号后面的部分

//         std::string fields_change = fields;
//         fields_change += ";"; // 末尾加上分号

//         std::replace(fields_change.begin(), fields_change.end(), ',', ';'); // 将逗号替换为分号

//         size_t end_pos = 0;
//         size_t start_pos = 0;
//         std::vector<std::string> parameter_array;

//         while ((end_pos = fields_change.find_first_of(';', end_pos + 1)) != std::string::npos)
//         {
//             start_pos = end_pos;

//             while (fields_change[start_pos] != ' ') // 从分号位置向前查找第一个空格的位置
//                 start_pos--;
//             std::string firstWord = fields_change.substr(start_pos, end_pos - start_pos);
//             parameter_array.push_back(firstWord);

//             // type_temp.erase(0, end_pos + 1); // 删除已经提取的部分
//         }

//         outputFile << "class " << typeName << " : public " << baseName << std::endl
//                    << "{" << std::endl;
//         outputFile << "public:" << std::endl;
//         outputFile << "    " << typeName << "(" << fields << ") : ";

//         for (auto &&i : parameter_array)
//         {
//             outputFile << i << "(" << i << "), ";
//         }

//         // Remove the last ", "
//         outputFile.seekp(-2, std::ios_base::end);

//         // Constructor
//         outputFile << " {}" << std::endl;

//         // Accept method
//         outputFile << "    std::string Accept(Visitor& visitor) override " << std::endl
//                    << "    {" << std::endl;
//         outputFile << "    return visitor.Visit" << typeName << "(*this);" << std::endl;
//         outputFile << "    }" << std::endl;

//         // Fields
//         outputFile << "   " << fields_change << std::endl;

//         outputFile << "};" << std::endl
//                    << std::endl;
//     }
//     outputFile << "#endif // EXPR_H" << std::endl;
// }

// int main()
// {
//     const std::string outputDir = "E:/GitRepository/Lox_cpp"; // Output directory
//     const std::string baseName = "Expr";                      // Base class name
//     const std::vector<std::string> types = {
//         "Assign   : Token name, Expr* value",
//         "Binary   : Expr* left, Token op, Expr* right",
//         "Call     : Expr* callee, Token paren, std::vector<Expr*> arguments",
//         "Get      : Expr* object, Token name",
//         "Grouping : Expr* expression",
//         "Literal  : Object value",
//         "Logical  : Expr* left, Token op, Expr* right",
//         "Set      : Expr* object, Token name, Expr* value",
//         "Super    : Token keyword, Token method",
//         "This     : Token keyword",
//         "Unary    : Token op, Expr* right",
//         "Variable : Token name"};

//     defineAst(outputDir, baseName, types);

//     return 0;
// }
