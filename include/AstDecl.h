#pragma once
#include <string>
namespace sammine_lang {
namespace AST {
using Identifier = std::string;
class ProgramAST;
class DefinitionAST;
class VarDefAST;
class ExternAST;
class FuncDefAST;
class PrototypeAST;
class TypedVarAST;
class Stmt;
class ExprAST;
class CallExprAST;
class BinaryExprAST;
class NumberExprAST;
class VariableExprAST;
class BlockAST;

} // namespace AST
} // namespace sammine_lang
