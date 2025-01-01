#pragma once
#include "AstBase.h"
#include "AstDecl.h"
#include "Lexer.h"
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

class ProgramAST : public AstBase {
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
};

class DefinitionAST : public AstBase {};

//! \brief A prototype to present "func func_name(...) -> type;"

//!
//!
class PrototypeAST : public AstBase {
public:
  llvm::Function *function;
  std::string functionName;
  std::string returnType;
  std::vector<std::unique_ptr<AST::TypedVarAST>> parameterVectors;

  PrototypeAST(std::string functionName, std::string returnType,
               std::vector<std::unique_ptr<AST::TypedVarAST>> parameterVectors)
      : functionName(functionName), returnType(std::move(returnType)),
        parameterVectors(std::move(parameterVectors)) {}

  virtual std::string getTreeName() override { return "PrototypeAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};

//! \brief A Function Definition that has the prototype and definition in terms
//! of a block
class ExternAST : public DefinitionAST {
public:
  std::unique_ptr<PrototypeAST> Prototype;

  ExternAST(std::unique_ptr<PrototypeAST> Prototype)
      : Prototype(std::move(Prototype)) {}

  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }

  virtual std::string getTreeName() override { return "ExternAST"; }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};
class FuncDefAST : public DefinitionAST {
public:
  std::unique_ptr<PrototypeAST> Prototype;
  std::unique_ptr<BlockAST> Block;

  FuncDefAST(std::unique_ptr<PrototypeAST> Prototype,
             std::unique_ptr<BlockAST> Block)
      : Prototype(std::move(Prototype)), Block(std::move(Block)) {}

  virtual std::string getTreeName() override { return "FuncDefAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};

//! \brief An AST to simulate a { } code block

//!
//!
class BlockAST : public AstBase {

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
};

class ExprAST : public AstBase {
public:
  inline static int personal_id_counter = 0;
};

//! \brief A variable definition: "var x = expression;"
class VarDefAST : public ExprAST {
public:
  std::unique_ptr<TypedVarAST> TypedVar;
  std::unique_ptr<ExprAST> Expression;

  explicit VarDefAST(std::unique_ptr<TypedVarAST> TypedVar,
                     std::unique_ptr<ExprAST> Expression)
      : TypedVar(std::move(TypedVar)), Expression(std::move(Expression)) {

        };

  virtual std::string getTreeName() override { return "VarDefAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};
class NumberExprAST : public ExprAST {
public:
  std::string number;

  virtual std::string getTreeName() override { return "NumberExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};

class BoolExprAST : public ExprAST {
public:
  bool b;
  BoolExprAST(bool b) : b(b) {}
  virtual std::string getTreeName() override { return "BoolExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};
class BinaryExprAST : public ExprAST {
public:
  std::shared_ptr<Token> Op;
  std::unique_ptr<ExprAST> LHS, RHS;
  BinaryExprAST(std::shared_ptr<Token> op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

  virtual std::string getTreeName() override { return "BinaryExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};

class CallExprAST : public ExprAST {

public:
  std::string functionName;
  std::vector<std::unique_ptr<AST::ExprAST>> arguments;
  CallExprAST(std::string functionName,
              std::vector<std::unique_ptr<AST::ExprAST>> arguments)
      : functionName(functionName), arguments(std::move(arguments)) {}

  virtual std::string getTreeName() override { return "CallExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
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
        elseBlockAST(std::move(elseBlockAST)) {}

  virtual std::string getTreeName() override { return "IfExpr"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};
class VariableExprAST : public ExprAST {
public:
  std::string variableName;
  VariableExprAST(std::shared_ptr<Token> var) : variableName(var->lexeme) {};
  VariableExprAST(std::string variableName) : variableName(variableName) {};

  virtual std::string getTreeName() override { return "VariableExprAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};

class TypedVarAST : public AstBase {
public:
  std::string name;
  std::string type;
  virtual std::string getTreeName() override { return "TypedVarAST"; }
  void accept_vis(ASTVisitor *visitor) override { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) override {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) override {
    visitor->postorder_walk(this);
  }
};

} // namespace AST
} // namespace sammine_lang
