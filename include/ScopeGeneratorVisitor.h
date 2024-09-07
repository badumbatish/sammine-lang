#pragma once
#include "AstBase.h"
#include <memory>

#include <map>
#include <set>

namespace sammine_lang::AST {
class LexicalScope {
  std::shared_ptr<LexicalScope> parent_scope;
  std::map<std::string, AST::Visitable &> scope_map;

public:
  explicit LexicalScope() {}
  LexicalScope(std::shared_ptr<LexicalScope> parent_scope)
      : parent_scope(parent_scope) {}
};
class ScopeGeneratorVisitor : public ASTVisitor {
  LexicalScope global_scope;
  std::set<std::string> symbols;
  ScopeGeneratorVisitor() {}
  LexicalScope get_scope_map();
};

} // namespace sammine_lang::AST
