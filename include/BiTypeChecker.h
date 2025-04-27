#pragma once

#include "AstBase.h"
#include "LexicalContext.h"
#include "Types.h"
#include <stack>
namespace sammine_lang {

namespace AST {

class TypingContext : public LexicalContext<Type> {};
class BiTypeCheckerVisitor : public ScopedASTVisitor,
                             public TypeCheckerVisitor {
  /// INFO: Ok let's talk about error propagation in this checker.
  /// the synthesize will error if I cannot get something out of id_map or
  /// typename_map (technically shouldn't happen)

  // We're gonna provide look up in different

public:
  std::stack<TypingContext> id_to_type;
  std::stack<TypingContext> typename_to_type;
  TypeMapOrdering type_map_ordering;
  virtual void enter_new_scope() override {
    id_to_type.push(TypingContext());
    typename_to_type.push(TypingContext());
  }
  virtual void exit_new_scope() override {
    id_to_type.pop();
    typename_to_type.pop();
  }
  BiTypeCheckerVisitor() { this->enter_new_scope(); }

  std::optional<Type> get_id_type(const std::string &str) {

    auto &id_name_top = id_to_type.top();
    if (id_name_top.queryName(str) == TypingContext::nameNotFound) {
      return std::nullopt;
    }
    return id_name_top.get_from_name(str);
  }
  std::optional<Type> get_typename_type(const std::string &str) {
    auto &typename_top = typename_to_type.top();
    if (typename_top.queryName(str) == TypingContext::nameNotFound) {
      return std::nullopt;
    }
    return typename_top.get_from_name(str);
  }

  // pre order

  virtual void preorder_walk(ProgramAST *ast) override;
  virtual void preorder_walk(VarDefAST *ast) override;
  virtual void preorder_walk(ExternAST *ast) override;
  virtual void preorder_walk(FuncDefAST *ast) override;
  virtual void preorder_walk(PrototypeAST *ast) override;
  virtual void preorder_walk(CallExprAST *ast) override;
  virtual void preorder_walk(BinaryExprAST *ast) override;
  virtual void preorder_walk(NumberExprAST *ast) override;
  virtual void preorder_walk(BoolExprAST *ast) override;
  virtual void preorder_walk(VariableExprAST *ast) override;
  virtual void preorder_walk(BlockAST *ast) override;
  virtual void preorder_walk(IfExprAST *ast) override;
  virtual void preorder_walk(TypedVarAST *ast) override;

  // post order
  virtual void postorder_walk(ProgramAST *ast) override;
  virtual void postorder_walk(VarDefAST *ast) override;
  virtual void postorder_walk(ExternAST *ast) override;
  virtual void postorder_walk(FuncDefAST *ast) override;
  virtual void postorder_walk(PrototypeAST *ast) override;
  virtual void postorder_walk(CallExprAST *ast) override;
  virtual void postorder_walk(BinaryExprAST *ast) override;
  virtual void postorder_walk(NumberExprAST *ast) override;
  virtual void postorder_walk(BoolExprAST *ast) override;
  virtual void postorder_walk(VariableExprAST *ast) override;
  virtual void postorder_walk(BlockAST *ast) override;
  virtual void postorder_walk(IfExprAST *ast) override;
  virtual void postorder_walk(TypedVarAST *ast) override;

  virtual Type synthesize(ProgramAST *ast) override;
  virtual Type synthesize(VarDefAST *ast) override;
  virtual Type synthesize(ExternAST *ast) override;
  virtual Type synthesize(FuncDefAST *ast) override;
  virtual Type synthesize(PrototypeAST *ast) override;
  virtual Type synthesize(CallExprAST *ast) override;
  virtual Type synthesize(BinaryExprAST *ast) override;
  virtual Type synthesize(NumberExprAST *ast) override;
  virtual Type synthesize(BoolExprAST *ast) override;
  virtual Type synthesize(VariableExprAST *ast) override;
  virtual Type synthesize(BlockAST *ast) override;
  virtual Type synthesize(IfExprAST *ast) override;
  virtual Type synthesize(TypedVarAST *ast) override;

  virtual bool check(ProgramAST *ast) override;
  virtual bool check(VarDefAST *ast) override;
  virtual bool check(ExternAST *ast) override;
  virtual bool check(FuncDefAST *ast) override;
  virtual bool check(PrototypeAST *ast) override;
  virtual bool check(CallExprAST *ast) override;
  virtual bool check(BinaryExprAST *ast) override;
  virtual bool check(NumberExprAST *ast) override;
  virtual bool check(BoolExprAST *ast) override;
  virtual bool check(VariableExprAST *ast) override;
  virtual bool check(BlockAST *ast) override;
  virtual bool check(IfExprAST *ast) override;
  virtual bool check(TypedVarAST *ast) override;
};
} // namespace AST
} // namespace sammine_lang
