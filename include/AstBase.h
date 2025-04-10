//
// Created by jjasmine on 3/9/24.
//

#pragma once
#include "AstDecl.h"
#include "Lexer.h"
#include "Types.h"
#include "Utilities.h"
namespace llvm {
class Value;

class Function;
} // namespace llvm

namespace sammine_lang {
namespace AST {
class Visitable;
class ASTVisitor;

class ASTVisitor : public sammine_util::Reportee {
public:
  virtual void visit(ProgramAST *ast);
  virtual void preorder_walk(ProgramAST *ast) = 0;
  virtual void postorder_walk(ProgramAST *ast) = 0;

  virtual void visit(VarDefAST *ast);
  virtual void preorder_walk(VarDefAST *ast) = 0;
  virtual void postorder_walk(VarDefAST *ast) = 0;

  virtual void visit(ExternAST *ast);
  virtual void preorder_walk(ExternAST *ast) = 0;
  virtual void postorder_walk(ExternAST *ast) = 0;

  virtual void visit(FuncDefAST *ast);
  virtual void preorder_walk(FuncDefAST *ast) = 0;
  virtual void postorder_walk(FuncDefAST *ast) = 0;

  virtual void visit(PrototypeAST *ast);
  virtual void preorder_walk(PrototypeAST *ast) = 0;
  virtual void postorder_walk(PrototypeAST *ast) = 0;

  virtual void visit(CallExprAST *ast);
  virtual void preorder_walk(CallExprAST *ast) = 0;
  virtual void postorder_walk(CallExprAST *ast) = 0;

  virtual void visit(BinaryExprAST *ast);
  virtual void preorder_walk(BinaryExprAST *ast) = 0;
  virtual void postorder_walk(BinaryExprAST *ast) = 0;

  virtual void visit(NumberExprAST *ast);
  virtual void preorder_walk(NumberExprAST *ast) = 0;
  virtual void postorder_walk(NumberExprAST *ast) = 0;

  virtual void visit(BoolExprAST *ast);
  virtual void preorder_walk(BoolExprAST *ast) = 0;
  virtual void postorder_walk(BoolExprAST *ast) = 0;

  virtual void visit(VariableExprAST *ast);
  virtual void preorder_walk(VariableExprAST *ast) = 0;
  virtual void postorder_walk(VariableExprAST *ast) = 0;

  virtual void visit(BlockAST *ast);
  virtual void preorder_walk(BlockAST *ast) = 0;
  virtual void postorder_walk(BlockAST *ast) = 0;

  virtual void visit(IfExprAST *ast);
  virtual void preorder_walk(IfExprAST *ast) = 0;
  virtual void postorder_walk(IfExprAST *ast) = 0;

  virtual void visit(TypedVarAST *ast);
  virtual void preorder_walk(TypedVarAST *ast) = 0;
  virtual void postorder_walk(TypedVarAST *ast) = 0;

  virtual ~ASTVisitor() = 0;
};

class ScopedASTVisitor : public ASTVisitor {
public:
  virtual void enter_new_scope() = 0;
  virtual void exit_new_scope() = 0;

  virtual void visit(ProgramAST *ast);

  virtual void visit(VarDefAST *ast);

  virtual void visit(ExternAST *ast);

  virtual void visit(FuncDefAST *ast);

  virtual void visit(PrototypeAST *ast);

  virtual void visit(CallExprAST *ast);

  virtual void visit(BinaryExprAST *ast);

  virtual void visit(NumberExprAST *ast);

  virtual void visit(BoolExprAST *ast);

  virtual void visit(VariableExprAST *ast);

  virtual void visit(BlockAST *ast);

  virtual void visit(IfExprAST *ast);

  virtual void visit(TypedVarAST *ast);
  virtual ~ScopedASTVisitor() = 0;
};

class TypeCheckerVisitor {
public:
  virtual Type synthesize(ProgramAST *ast) = 0;
  virtual Type synthesize(VarDefAST *ast) = 0;
  virtual Type synthesize(ExternAST *ast) = 0;
  virtual Type synthesize(FuncDefAST *ast) = 0;
  virtual Type synthesize(PrototypeAST *ast) = 0;
  virtual Type synthesize(CallExprAST *ast) = 0;
  virtual Type synthesize(BinaryExprAST *ast) = 0;
  virtual Type synthesize(NumberExprAST *ast) = 0;
  virtual Type synthesize(BoolExprAST *ast) = 0;
  virtual Type synthesize(VariableExprAST *ast) = 0;
  virtual Type synthesize(BlockAST *ast) = 0;
  virtual Type synthesize(IfExprAST *ast) = 0;
  virtual Type synthesize(TypedVarAST *ast) = 0;

  virtual bool check(ProgramAST *ast) = 0;
  virtual bool check(VarDefAST *ast) = 0;
  virtual bool check(ExternAST *ast) = 0;
  virtual bool check(FuncDefAST *ast) = 0;
  virtual bool check(PrototypeAST *ast) = 0;
  virtual bool check(CallExprAST *ast) = 0;
  virtual bool check(BinaryExprAST *ast) = 0;
  virtual bool check(NumberExprAST *ast) = 0;
  virtual bool check(BoolExprAST *ast) = 0;
  virtual bool check(VariableExprAST *ast) = 0;
  virtual bool check(BlockAST *ast) = 0;
  virtual bool check(IfExprAST *ast) = 0;
  virtual bool check(TypedVarAST *ast) = 0;

  virtual ~TypeCheckerVisitor() = 0;
};

class Visitable {
public:
  virtual ~Visitable() = default;
  virtual void accept_vis(ASTVisitor *visitor) = 0;
  virtual void walk_with_preorder(ASTVisitor *visitor) = 0;
  virtual void walk_with_postorder(ASTVisitor *visitor) = 0;
  virtual Type accept_synthesis(TypeCheckerVisitor *visitor) = 0;
  virtual bool accept_check(TypeCheckerVisitor *visitor) = 0;
  virtual std::string getTreeName() = 0;
};

class AstBase : public Visitable {
  void change_location(sammine_util::Location loc) {
    if (first_location) {
      this->location = loc;
      first_location = false;
    } else
      this->location |= loc;
  }

  bool first_location = true;

protected:
  sammine_util::Location location;

public:
  llvm::Value *val;
  Type type = Type::NonExistent();
  AstBase *join_location(AstBase *ast) {
    if (!ast)
      sammine_util::abort(fmt::format("ast cannot be nullptr"));
    else
      change_location(ast->location);

    return this;
  }

  AstBase *join_location(std::shared_ptr<Token> tok) {

    if (!tok)
      sammine_util::abort(fmt::format("tok cannot be nullptr"));
    else
      change_location(tok->location);

    return this;
  }
  AstBase *join_location(sammine_util::Location location) {

    change_location(location);

    return this;
  }
  sammine_util::Location get_location() { return this->location; }
};
} // namespace AST
} // namespace sammine_lang
