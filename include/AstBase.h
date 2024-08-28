//
// Created by jjasmine on 3/9/24.
//

#ifndef SAMMINE_LANG_ASTBASE_H
#define SAMMINE_LANG_ASTBASE_H
#include "AstDecl.h"
namespace llvm {
class Value;

class Function;
} // namespace llvm

namespace sammine_lang {
namespace AST {
class Visitable;
class ASTVisitor;

class ASTVisitor {
public:
  virtual void visit(ProgramAST *ast);
  virtual void preorder_walk(ProgramAST *ast) = 0;
  virtual void postorder_walk(ProgramAST *ast) = 0;

  virtual void visit(VarDefAST *ast);
  virtual void preorder_walk(VarDefAST *ast) = 0;
  virtual void postorder_walk(VarDefAST *ast) = 0;

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

  virtual void visit(VariableExprAST *ast);
  virtual void preorder_walk(VariableExprAST *ast) = 0;
  virtual void postorder_walk(VariableExprAST *ast) = 0;

  virtual void visit(BlockAST *ast);
  virtual void preorder_walk(BlockAST *ast) = 0;
  virtual void postorder_walk(BlockAST *ast) = 0;

  virtual void visit(TypedVarAST *ast);
  virtual void preorder_walk(TypedVarAST *ast) = 0;
  virtual void postorder_walk(TypedVarAST *ast) = 0;
};
class Visitable {
public:
  virtual ~Visitable() = default;
  virtual void accept_vis(ASTVisitor *visitor) = 0;
  virtual void walk_with_preorder(ASTVisitor *visitor) = 0;
  virtual void walk_with_postorder(ASTVisitor *visitor) = 0;
  virtual std::string getTreeName() = 0;
};

class AstBase : public Visitable {
public:
  llvm::Value *val;
};
} // namespace AST
} // namespace sammine_lang
#endif // SAMMINE_LANG_ASTBASE_H
