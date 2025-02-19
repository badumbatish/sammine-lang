#pragma once
#include "Ast.h"
#include "AstBase.h"
#include "Utilities.h"
#include "tl/expected.hpp"
#include <memory>

#include <map>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>

namespace sammine_lang::AST {

// A simple scoping class, doesn't differentiate between different names, like
// variable name, func name and all that
class LexicalScope {
  std::shared_ptr<LexicalScope> parent_scope;
  std::set<std::string> symbols;
  std::unordered_map<std::string, sammine_util::Location> symbols_to_loc;

public:
  explicit LexicalScope() {}
  LexicalScope(std::shared_ptr<LexicalScope> parent_scope)
      : parent_scope(parent_scope) {}

  enum NameQueryResult {
    nameFound,
    nameNotFound,
  };
  void registerNameLocation(const std::string &name, sammine_util::Location l) {
    symbols.insert(name);
    symbols_to_loc[name] = l;
  }
  NameQueryResult queryName(std::string name) const {
    return symbols.contains(name) ? nameFound : nameNotFound;
  }

  sammine_util::Location get_location(const std::string &name) {
    return symbols_to_loc.at(name);
  }

  NameQueryResult recursiveQueryName(const std::string &name) const {
    if (symbols.find(name) != symbols.end())
      return nameFound;
    else if (parent_scope == nullptr)
      return nameNotFound;
    return parent_scope->recursiveQueryName(name);
  }
}; // namespace sammine_lang::AST

class ScopeGeneratorVisitor : public ASTVisitor {
public:
  std::stack<LexicalScope> scope_stack;
  ScopeGeneratorVisitor() { scope_stack.push(LexicalScope()); }
  LexicalScope get_scope_map();

  // INFO: CheckAndReg means: Check if there's redefinition, if not, register
  // INFO: Check for castable means: Check if the name existed, if not, register

  // pre order

  // INFO: Nothing here
  void preorder_walk(ProgramAST *ast) override;

  // INFO: CheckAndReg variable name
  void preorder_walk(VarDefAST *ast) override;

  // INFO: CheckAndReg extern name
  void preorder_walk(ExternAST *ast) override;
  // INFO: CheckAndReg function name, enter new block
  void preorder_walk(FuncDefAST *ast) override;
  // INFO: CheckAndReg all variable name, which should only clash if you have
  // the same names in prototype
  void preorder_walk(PrototypeAST *ast) override;
  // INFO: Check
  void preorder_walk(CallExprAST *ast) override;
  void preorder_walk(BinaryExprAST *ast) override;
  void preorder_walk(NumberExprAST *ast) override;
  void preorder_walk(BoolExprAST *ast) override;

  // INFO: Check
  void preorder_walk(VariableExprAST *ast) override;
  void preorder_walk(BlockAST *ast) override;
  void preorder_walk(IfExprAST *ast) override;
  void preorder_walk(TypedVarAST *ast) override;

  // post order
  void postorder_walk(ProgramAST *ast) override;
  void postorder_walk(VarDefAST *ast) override;
  void postorder_walk(ExternAST *ast) override;
  // INFO: Pop the scope
  void postorder_walk(FuncDefAST *ast) override;
  void postorder_walk(PrototypeAST *ast) override;
  void postorder_walk(CallExprAST *ast) override;
  void postorder_walk(BinaryExprAST *ast) override;
  void postorder_walk(NumberExprAST *ast) override;
  void postorder_walk(BoolExprAST *ast) override;
  void postorder_walk(VariableExprAST *ast) override;
  void postorder_walk(BlockAST *ast) override;
  void postorder_walk(IfExprAST *ast) override;
  void postorder_walk(TypedVarAST *ast) override;
};

} // namespace sammine_lang::AST
