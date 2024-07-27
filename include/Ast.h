#ifndef SAMMINE_LANG_AST_H
#define SAMMINE_LANG_AST_H
#include "AstBase.h"
#include "Lexer.h"
#include <memory>
#include <string>
#include <vector>

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

class ProgramAST : public AstBase {
public:
  std::vector<std::unique_ptr<DefinitionAST>> DefinitionVec;
  virtual std::string getTreeName() override { return "ProgramAST"; }
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

  virtual std::string getTreeName() override { return "VarDefAST"; }
};

//! \brief A prototype to present "func func_name(...) -> type;"

//!
//!
class PrototypeAST : public AstBase {
public:
  llvm::Function *function;
  std::string functionName;
  std::string returnType;
  std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>>
      parameterVectors;

  PrototypeAST(std::string functionName, std::string returnType,
               std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>>
                   parameterVectors)
      : functionName(functionName), returnType(std::move(returnType)),
        parameterVectors(std::move(parameterVectors)) {}

  virtual std::string getTreeName() override { return "PrototypeAST"; }
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

  virtual std::string getTreeName() override { return "FuncDefAST"; }
};

//! \brief An AST to simulate a { } code block

//!
//!
class BlockAST : public AstBase {
public:
  std::vector<std::unique_ptr<ExprAST>> Statements;
  virtual std::string getTreeName() override { return "BlockAST"; }
};

class ExprAST : public AstBase {};

class NumberExprAST : public ExprAST {
public:
  std::string number;

  virtual std::string getTreeName() override { return "NumberExprAST"; }
};

class BinaryExprAST : public ExprAST {
public:
  std::shared_ptr<Token> Op;
  std::unique_ptr<ExprAST> LHS, RHS;
  BinaryExprAST(std::shared_ptr<Token> op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

  virtual std::string getTreeName() override { return "BinaryExprAST"; }
};

class CallExprAST : public ExprAST {

public:
  std::string functionName;
  std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>> arguments;
  CallExprAST(
      std::string functionName,
      std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>> arguments)
      : functionName(functionName), arguments(std::move(arguments)) {}

  virtual std::string getTreeName() override { return "CallExprAST"; }
};

class VariableExprAST : public ExprAST {
public:
  std::string variableName;
  VariableExprAST(std::shared_ptr<Token> var) : variableName(var->lexeme){};

  virtual std::string getTreeName() override { return "VariableExprAST"; }
};

class TypedVarAST : public AstBase {
public:
  std::string name;
  std::string type;
  virtual std::string getTreeName() override { return "TypedVarAST"; }
};

} // namespace AST
} // namespace sammine_lang

#endif // SAMMINE_LANG_AST_H
