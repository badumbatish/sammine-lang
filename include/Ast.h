#include "AstBase.h"
#include "Lexer.h"
#include <memory>
#include <string>
#include <vector>
#ifndef SAMMINE_LANG_AST_H
#define SAMMINE_LANG_AST_H
namespace sammine_lang {
namespace AST {

using Identifier = std::string;
class ProgramAST;

class DefinitionAST;
class VarDefAST;
class FuncDefAST;

class PrototypeAST;
class TypedVarAST;

class ExprAST;
class CallExprAST;
class BinaryExprAST;
class NumberExprAST;
class VariableExprAST;

class BlockAST;
class StmtAST;

class SimpleStmtAST;
class IfStmtAST;


class ProgramAST : public AstBase {
public:
  std::vector<std::unique_ptr<DefinitionAST>> DefinitionVec;
};

class DefinitionAST : public AstBase {};

//! \brief A variable definition: "var x = expression;"
class VarDefAST : public DefinitionAST {
public:
  std::unique_ptr<TypedVarAST> TypedVar;
  std::unique_ptr<ExprAST> Expression;

  VarDefAST(std::unique_ptr<TypedVarAST> TypedVar,
            std::unique_ptr<ExprAST> Expression)
      : TypedVar(std::move(TypedVar)), Expression(std::move(Expression)){};
};

//! \brief A prototype to present "func func_name(...) -> type;"

//!
//!
class PrototypeAST : public AstBase {
public:
  std::string functionName;
  std::string returnType;
  std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>>
      parameterVectors;

  PrototypeAST(std::string functionName, std::string returnType,
               std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>>
                   parameterVectors)
      : functionName(functionName), returnType(std::move(returnType)),
        parameterVectors(std::move(parameterVectors)) {}
};

//! \brief A Function Definition that has the prototype and definition in terms
//! of a block
class FuncDefAST : public DefinitionAST {
public:
  std::unique_ptr<PrototypeAST> Prototype;
  std::unique_ptr<BlockAST> Block;

  FuncDefAST(std::unique_ptr<PrototypeAST> Prototype,
             std::unique_ptr<BlockAST> Block)
      : Prototype(std::move(Prototype)), Block(std::move(Block)) {}
};

//! \brief An AST to simulate a { } code block

//!
//!
class BlockAST : public AstBase {
public:
  std::vector<std::unique_ptr<StmtAST>> Statements;
  std::unique_ptr<ExprAST> returnStmt;
};

class StmtAST : public AstBase {};

class SimpleStmtAST : public StmtAST {};

class IfStmtAST : public StmtAST {};

class ExprAST : public AstBase {};

class NumberExprAST : public ExprAST {
public:
  std::string number;
};

class BinaryExprAST : public ExprAST {
public:
  std::shared_ptr<Token> Op;
  std::unique_ptr<ExprAST> LHS, RHS;
  BinaryExprAST(std::shared_ptr<Token> op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST {

public:
  std::string functionName;
  std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>> arguments;
  CallExprAST(
      std::string functionName,
      std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>> arguments)
      : functionName(functionName), arguments(std::move(arguments)) {}
};
class TypedVarAST : public AstBase {
public:
  std::string name;
  std::string type;
};

} // namespace AST
} // namespace sammine_lang

#endif // SAMMINE_LANG_AST_H
