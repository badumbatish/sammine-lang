#pragma once

#include "ast/Ast.h"
#include "ast/AstBase.h"
#include "typecheck/Types.h"
#include "util/LexicalContext.h"
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
  // INFO: x, y, z
  LexicalStack<Type> id_to_type;

  // INFO: i64, f64 bla bla bla
  LexicalStack<Type> typename_to_type;
  TypeMapOrdering type_map_ordering;
  virtual void enter_new_scope() override {
    id_to_type.push_context();
    typename_to_type.push_context();

    typename_to_type.registerNameT("i64", Type::I64_t());
    typename_to_type.registerNameT("f64", Type::F64_t());
    typename_to_type.registerNameT("bool", Type::Bool());
    typename_to_type.registerNameT("unit", Type::Unit());
  }
  virtual void exit_new_scope() override {
    id_to_type.pop();
    typename_to_type.pop();
  }
  BiTypeCheckerVisitor() { this->enter_new_scope(); }

  std::optional<Type> get_type_from_id(const std::string &str) {

    auto &id_name_top = id_to_type.top();
    if (id_name_top.queryName(str) == nameNotFound) {
      sammine_util::abort("should not happen");
    }
    return id_name_top.get_from_name(str);
  }
  std::optional<Type> get_typename_type(const std::string &str) {
    auto &typename_top = typename_to_type.top();
    if (typename_top.queryName(str) == nameNotFound) {
      return std::nullopt;
    }
    return typename_top.get_from_name(str);
  }

  // pre order

  virtual void preorder_walk(ProgramAST *ast) override;
  virtual void preorder_walk(VarDefAST *ast) override;
  virtual void preorder_walk(ExternAST *ast) override;
  virtual void preorder_walk(FuncDefAST *ast) override;
  virtual void preorder_walk(RecordDefAST *ast) override;
  virtual void preorder_walk(PrototypeAST *ast) override;
  virtual void preorder_walk(CallExprAST *ast) override;
  virtual void preorder_walk(ReturnExprAST *ast) override;
  virtual void preorder_walk(BinaryExprAST *ast) override;
  virtual void preorder_walk(NumberExprAST *ast) override;
  virtual void preorder_walk(StringExprAST *ast) override;
  virtual void preorder_walk(BoolExprAST *ast) override;
  virtual void preorder_walk(UnitExprAST *ast) override;
  virtual void preorder_walk(VariableExprAST *ast) override;
  virtual void preorder_walk(BlockAST *ast) override;
  virtual void preorder_walk(IfExprAST *ast) override;
  virtual void preorder_walk(TypedVarAST *ast) override;

  // post order
  virtual void postorder_walk(ProgramAST *ast) override;
  virtual void postorder_walk(VarDefAST *ast) override;
  virtual void postorder_walk(ExternAST *ast) override;
  virtual void postorder_walk(FuncDefAST *ast) override;
  virtual void postorder_walk(RecordDefAST *ast) override;
  virtual void postorder_walk(PrototypeAST *ast) override;
  virtual void postorder_walk(CallExprAST *ast) override;
  virtual void postorder_walk(ReturnExprAST *ast) override;
  virtual void postorder_walk(BinaryExprAST *ast) override;
  virtual void postorder_walk(NumberExprAST *ast) override;
  virtual void postorder_walk(StringExprAST *ast) override;
  virtual void postorder_walk(BoolExprAST *ast) override;
  virtual void postorder_walk(UnitExprAST *ast) override;
  virtual void postorder_walk(VariableExprAST *ast) override;
  virtual void postorder_walk(BlockAST *ast) override;
  virtual void postorder_walk(IfExprAST *ast) override;
  virtual void postorder_walk(TypedVarAST *ast) override;

  virtual Type synthesize(ProgramAST *ast) override;
  virtual Type synthesize(VarDefAST *ast) override;
  virtual Type synthesize(ExternAST *ast) override;
  virtual Type synthesize(FuncDefAST *ast) override;
  virtual Type synthesize(RecordDefAST *ast) override;
  virtual Type synthesize(PrototypeAST *ast) override;
  virtual Type synthesize(CallExprAST *ast) override;
  virtual Type synthesize(ReturnExprAST *ast) override;
  virtual Type synthesize(BinaryExprAST *ast) override;
  virtual Type synthesize(NumberExprAST *ast) override;
  virtual Type synthesize(UnitExprAST *ast) override;
  virtual Type synthesize(StringExprAST *ast) override;
  virtual Type synthesize(BoolExprAST *ast) override;
  virtual Type synthesize(VariableExprAST *ast) override;
  virtual Type synthesize(BlockAST *ast) override;
  virtual Type synthesize(IfExprAST *ast) override;
  virtual Type synthesize(TypedVarAST *ast) override;

  Type get_type_from_type_lexeme(const std::string &type_lexeme,
                                 sammine_util::Location location) {
    if (type_lexeme.empty()) {
      return Type::NonExistent();
    }
    auto get_type_opt = this->get_typename_type(type_lexeme);

    if (!get_type_opt.has_value()) {
      this->add_error(location,
                      fmt::format("Type '{}' not found in the current scope.",
                                  type_lexeme));
      return Type::Poisoned();
    } else
      return get_type_opt.value();
  }
};
} // namespace AST
} // namespace sammine_lang
