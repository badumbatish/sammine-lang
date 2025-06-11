#pragma once
#include <string>
namespace sammine_lang {
namespace AST {
using Identifier = std::string;
class Printable;
class ProgramAST;
class DefinitionAST;
class VarDefAST;
class ExternAST;
class FuncDefAST;
class RecordDefAST;
class PrototypeAST;
class TypedVarAST;
class Stmt;
class ExprAST;
class CallExprAST;
class UnitExprAST;
class ReturnExprAST;
class BinaryExprAST;
class NumberExprAST;
class StringExprAST;
class BoolExprAST;
class VariableExprAST;
class BlockAST;
class IfExprAST;

} // namespace AST
} // namespace sammine_lang
