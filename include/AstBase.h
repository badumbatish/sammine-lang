//
// Created by jjasmine on 3/9/24.
//

#ifndef SAMMINE_LANG_ASTBASE_H
#define SAMMINE_LANG_ASTBASE_H
#include "llvm/IR/Value.h"

namespace sammine_lang {
namespace AST {
class Visitable;
class ASTVisitor;

class ASTVisitor {
public:
  virtual void visit(Visitable *visitable) = 0;
  virtual void preorder_walk(Visitable *visitable) {}
  virtual void postorder_walk(Visitable *visitable) {}
};
class Visitable {
public:
  virtual ~Visitable() = default;
  virtual void accept_vis(ASTVisitor *visitor) { visitor->visit(this); }
  virtual void walk_with_preorder(ASTVisitor *visitor) {
    visitor->preorder_walk(this);
  }
  virtual void walk_with_postorder(ASTVisitor *visitor) {
    visitor->postorder_walk(this);
  }
  virtual std::string getTreeName() = 0;
};

class AstBase : public Visitable {
public:
  llvm::Value *val;
};
} // namespace AST
} // namespace sammine_lang
#endif // SAMMINE_LANG_ASTBASE_H
