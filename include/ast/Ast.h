#pragma once
#include "ast/AstBase.h"
#include "ast/AstDecl.h"
#include "util/Utilities.h"
#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

//! \file Ast.h
//! \brief Defined the AST Node classes (ProgramAST, RecordDefAST, FuncDefAST)
//! and a visitor interface for traversing the AST

namespace sammine_lang {

namespace AST {
using Identifier = std::string;
class Printable {};

class DefinitionAST : public AstBase, public Printable {};

class ProgramAST : public AstBase, public Printable {
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
};

class TypedVarAST : public AstBase, public Printable {
public:
  std::string name;
  std::string type_lexeme;

  explicit TypedVarAST(std::shared_ptr<Token> name,
                       std::shared_ptr<Token> type) {
    this->join_location(name)->join_location(type);
    if (name)
      this->name = name->lexeme;
    if (type)
      this->type_lexeme = type->lexeme;
  }
  explicit TypedVarAST(std::shared_ptr<Token> name) {
    this->join_location(name);
    if (name)
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
};

//! \brief A prototype to present "func func_name(...) -> type;"

//!
//!
class PrototypeAST : public AstBase, public Printable {
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
};
class ExprAST : public AstBase, public Printable {
public:
  ~ExprAST() = default;
};

//! \brief An AST to simulate a { } code block
//!
//!
class BlockAST : public AstBase, public Printable {

public:
  std::vector<std::unique_ptr<ExprAST>> Statements;
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
};

class FuncDefAST : public DefinitionAST {
public:
  ~FuncDefAST() = default;
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
};

// Record id { typed_var }
class RecordDefAST : public DefinitionAST {
public:
  std::string record_name;
  std::vector<std::unique_ptr<TypedVarAST>> record_members;
  virtual std::string getTreeName() override { return "RecordDefAST"; }

  explicit RecordDefAST(std::shared_ptr<Token> record_id,
                        decltype(record_members) record_members)
      : record_members(std::move(record_members)) {
    if (record_id)
      record_name = record_id->lexeme;

    this->join_location(record_id);
    for (auto &m : record_members)
      this->join_location(m.get());
  }
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
};
class StringExprAST : public ExprAST {
public:
  std::string string_content;

  explicit StringExprAST(std::shared_ptr<Token> t) {
    assert(t);
    join_location(t);
    string_content = t->lexeme;
  }
  virtual std::string getTreeName() override { return "StringExprAST"; }
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
};
class ReturnExprAST : public ExprAST {

public:
  bool is_implicit;
  std::unique_ptr<ExprAST> return_expr;
  ReturnExprAST(std::shared_ptr<Token> return_tok,
                std::unique_ptr<ExprAST> return_expr)
      : is_implicit(false), return_expr(std::move(return_expr)) {
    if (return_expr == nullptr) {
      this->join_location(return_tok);
    } else {
      this->join_location(return_tok)->join_location(this->return_expr.get());
    }
  }

  ReturnExprAST(std::unique_ptr<ExprAST> return_expr)
      : is_implicit(true), return_expr(std::move(return_expr)) {
    this->join_location(this->return_expr.get());
  }

  virtual std::string getTreeName() override { return "ReturnExprAST"; }
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
};
class CallExprAST : public ExprAST {

public:
  std::string functionName;
  std::vector<std::unique_ptr<AST::ExprAST>> arguments;
  explicit CallExprAST(
      std::shared_ptr<Token> functionName,
      std::vector<std::unique_ptr<AST::ExprAST>> arguments = {}) {
    join_location(functionName);
    if (functionName)
      this->functionName = functionName->lexeme;

    for (auto &arg : arguments)
      if (arg)
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
};

class UnitExprAST : public ExprAST {

public:
  bool is_implicit;

  explicit UnitExprAST(std::shared_ptr<Token> left_paren,
                       std::shared_ptr<Token> right_paren)
      : is_implicit(false) {
    assert(left_paren);
    assert(right_paren);
    this->join_location(left_paren)->join_location(right_paren);
  };
  explicit UnitExprAST() : is_implicit(true) {}

  virtual std::string getTreeName() override { return "UnitExpr"; }
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
};
class VariableExprAST : public ExprAST {
public:
  std::string variableName;
  VariableExprAST(std::shared_ptr<Token> var) {
    join_location(var);
    if (var)
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
};
} // namespace AST
} // namespace sammine_lang
