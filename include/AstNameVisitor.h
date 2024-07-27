
#ifndef SAMMINE_LANG_ASTNAMEVISITOR_H
#define SAMMINE_LANG_ASTNAMEVISITOR_H
#include "Ast.h"
#include "AstBase.h"

namespace sammine_lang::AST {
class AstNameVisitor : public ASTVisitor {
private:
public:
  std::vector<std::string> PreOrderNames;
  std::vector<std::string> PostOrderNames;
  AstNameVisitor() {}

  virtual void preorder_walk(Visitable *visitable) override {
    PreOrderNames.push_back(visitable->getTreeName());
  }
};
} // namespace sammine_lang::AST
#endif // SAMMINE_LANG_ASTNAMEVISITOR_H
