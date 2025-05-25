#pragma once
#include "AstBase.h"
#include "AstDecl.h"
#include "Lexer.h"
#include "Utilities.h"
#include <cassert>
#include <cstddef>
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
class Printable {};

class ProgramAST : public AstBase, Printable {
public:
  std::vector<std::unique_ptr<DefinitionAST>> DefinitionVec;
  virtual std::string getTreeName() override { return "ProgramAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

class DefinitionAST : public AstBase, Printable {};

class TypedVarAST : public AstBase, Printable {
public:
  std::string name;
  std::string type_lexeme;

  explicit TypedVarAST(std::shared_ptr<Token> name,
                       std::shared_ptr<Token> type) {
    assert(name);
    assert(type);
    this->join_location(name)->join_location(type);
    this->name = name->lexeme;
    this->type_lexeme = type->lexeme;
  }
  explicit TypedVarAST(std::shared_ptr<Token> name) {
    assert(name);
    this->join_location(name);
    this->name = name->lexeme;
  }
  virtual std::string getTreeName() override { return "TypedVarAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }

  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

//! \brief A prototype to present "func func_name(...) -> type;"

//!
//!
class PrototypeAST : public AstBase, Printable {
public:
  llvm::Function *function;
  std::string functionName;
  std::string returnType;
  std::vector<std::unique_ptr<AST::TypedVarAST>> parameterVectors;

  explicit PrototypeAST(
      std::shared_ptr<Token> functionName, std::shared_ptr<Token> returnType,
      std::vector<std::unique_ptr<AST::TypedVarAST>> parameterVectors) {
    assert(functionName);
    assert(returnType);
    this->functionName = functionName->lexeme;
    this->returnType = returnType->lexeme;
    this->join_location(functionName)->join_location(returnType);

    this->parameterVectors = std::move(parameterVectors);

    for (size_t i = 0; i < this->parameterVectors.size(); i++)
      assert(this->parameterVectors[i]);
    for (size_t i = 0; i < this->parameterVectors.size(); i++)
      this->join_location(this->parameterVectors[i]->get_location());
  }

  explicit PrototypeAST(
      std::shared_ptr<Token> functionName,
      std::vector<std::unique_ptr<AST::TypedVarAST>> parameterVectors) {
    assert(functionName);
    this->functionName = functionName->lexeme;
    this->join_location(functionName);

    this->parameterVectors = std::move(parameterVectors);

    for (size_t i = 0; i < this->parameterVectors.size(); i++)
      assert(this->parameterVectors[i]);
    for (size_t i = 0; i < this->parameterVectors.size(); i++)
      this->join_location(this->parameterVectors[i]->get_location());
  }
  virtual std::string getTreeName() override { return "PrototypeAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

//! \brief A Function Definition that has the prototype and definition in terms
//! of a block
class ExternAST : public DefinitionAST {
public:
  std::unique_ptr<PrototypeAST> Prototype;

  ExternAST(std::unique_ptr<PrototypeAST> Prototype)
      : Prototype(std::move(Prototype)) {
    this->join_location(this->Prototype.get());
  }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }

  virtual std::string getTreeName() override { return "ExternAST"; }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

//! \brief An AST to simulate a { } code block
//!
//!
class BlockAST : public AstBase, Printable {

public:
  std::vector<std::unique_ptr<ExprAST>> Statements;
  inline static size_t scope_id_counter = 0;
  virtual std::string getTreeName() override { return "BlockAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

class FuncDefAST : public DefinitionAST {
public:
  std::unique_ptr<PrototypeAST> Prototype;
  std::unique_ptr<BlockAST> Block;

  FuncDefAST(std::unique_ptr<PrototypeAST> Prototype,
             std::unique_ptr<BlockAST> Block)
      : Prototype(std::move(Prototype)), Block(std::move(Block)) {
    this->join_location(this->Prototype.get())
        ->join_location(this->Block.get());
  }

  virtual std::string getTreeName() override { return "FuncDefAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

class ExprAST : public AstBase, Printable {
public:
  inline static int personal_id_counter = 0;
};

//! \brief A variable definition: "var x = expression;"
class VarDefAST : public ExprAST {
public:
  std::unique_ptr<TypedVarAST> TypedVar;
  std::unique_ptr<ExprAST> Expression;

  explicit VarDefAST(std::shared_ptr<Token> let,
                     std::unique_ptr<TypedVarAST> TypedVar,
                     std::unique_ptr<ExprAST> Expression)
      : TypedVar(std::move(TypedVar)), Expression(std::move(Expression)) {

    this->join_location(let)
        ->join_location(this->TypedVar.get())
        ->join_location(this->Expression.get());
  };

  virtual std::string getTreeName() override { return "VarDefAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};
class NumberExprAST : public ExprAST {
public:
  std::string number;

  explicit NumberExprAST(std::shared_ptr<Token> t) {
    assert(t);
    join_location(t);
    number = t->lexeme;
  }
  virtual std::string getTreeName() override { return "NumberExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

class BoolExprAST : public ExprAST {
public:
  bool b;
  BoolExprAST(bool b, sammine_util::Location loc) : b(b) {
    this->location = loc;
  }
  virtual std::string getTreeName() override { return "BoolExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};
class BinaryExprAST : public ExprAST {
public:
  std::shared_ptr<Token> Op;
  std::unique_ptr<ExprAST> LHS, RHS;
  BinaryExprAST(std::shared_ptr<Token> op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {
    this->join_location(this->Op)
        ->join_location(this->LHS.get())
        ->join_location(this->RHS.get());
  }

  virtual std::string getTreeName() override { return "BinaryExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

class CallExprAST : public ExprAST {

public:
  std::string functionName;
  std::vector<std::unique_ptr<AST::ExprAST>> arguments;
  CallExprAST(std::shared_ptr<Token> functionName,
              std::vector<std::unique_ptr<AST::ExprAST>> arguments) {
    assert(functionName);
    join_location(functionName);
    this->functionName = functionName->lexeme;

    for (auto &arg : arguments)
      this->join_location(arg.get());
    this->arguments = std::move(arguments);
  }

  virtual std::string getTreeName() override { return "CallExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

class IfExprAST : public ExprAST {
public:
  std::unique_ptr<ExprAST> bool_expr;
  std::unique_ptr<BlockAST> thenBlockAST, elseBlockAST;
  explicit IfExprAST(std::unique_ptr<ExprAST> bool_expr,
                     std::unique_ptr<BlockAST> thenBlockAST,
                     std::unique_ptr<BlockAST> elseBlockAST)
      : bool_expr(std::move(bool_expr)), thenBlockAST(std::move(thenBlockAST)),
        elseBlockAST(std::move(elseBlockAST)) {
    this->join_location(this->bool_expr.get())
        ->join_location(this->thenBlockAST.get())
        ->join_location(this->elseBlockAST.get());
  }

  virtual std::string getTreeName() override { return "IfExpr"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};
class VariableExprAST : public ExprAST {
public:
  std::string variableName;
  VariableExprAST(std::shared_ptr<Token> var) {
    assert(var);
    join_location(var);
    variableName = var->lexeme;
  };

  virtual std::string getTreeName() override { return "VariableExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) override {
    return visitor->synthesize(this);
  }
  virtual bool accept_check(TypeCheckerVisitor *visitor) override {
    return visitor->check(this);
  }
};

} // namespace AST
} // namespace sammine_lang
