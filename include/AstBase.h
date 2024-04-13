//
// Created by jjasmine on 3/9/24.
//

#ifndef SAMMINE_LANG_ASTBASE_H
#define SAMMINE_LANG_ASTBASE_H
#include <llvm/IR/Value.h>

namespace sammine_lang {
namespace AST {
class Visitable;
class ASTVisitor;

class ASTVisitor {
public:
  virtual void visit(Visitable *visitable) = 0;
};
class Visitable {
public:
  virtual ~Visitable() = default;
  virtual void accept_vis(ASTVisitor *visitor) { visitor->visit(this); }
};

class AstBase : public Visitable {
public:
  llvm::Value* val;
};
} // namespace AST
} // namespace sammine_lang
#endif // SAMMINE_LANG_ASTBASE_H
