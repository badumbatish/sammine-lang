#include "ast/Ast.h"
#include "ast/AstBase.h"
#include "fmt/base.h"
#include "fmt/format.h"
#include <cstdio>
namespace sammine_lang::AST {

class AstPrinterVisitor : public ScopedASTVisitor {
  std::string rep = "";
  std::string current_tabs = "";
  std::string &tabs();
  void generic_preprintln(AstBase *ast);
  void generic_preprint(AstBase *ast);
  void generic_postprint();

public:
  virtual void enter_new_scope() override { current_tabs += "  "; }
  virtual void exit_new_scope() override { current_tabs += "  "; }

  virtual void visit(ProgramAST *ast) override;

  virtual void visit(VarDefAST *ast) override;

  virtual void visit(ExternAST *ast) override;

  virtual void visit(FuncDefAST *ast) override;

  virtual void visit(PrototypeAST *ast) override;

  virtual void visit(CallExprAST *ast) override;

  virtual void visit(BinaryExprAST *ast) override;

  virtual void visit(NumberExprAST *ast) override;

  virtual void visit(BoolExprAST *ast) override;

  virtual void visit(VariableExprAST *ast) override;

  virtual void visit(BlockAST *ast) override;
  virtual void visit(ReturnExprAST *ast) override;

  virtual void visit(IfExprAST *ast) override;

  virtual void visit(TypedVarAST *ast) override;
  // pre order
  virtual void preorder_walk(ProgramAST *ast) override;
  virtual void preorder_walk(VarDefAST *ast) override;
  virtual void preorder_walk(ExternAST *ast) override;
  virtual void preorder_walk(FuncDefAST *ast) override;
  virtual void preorder_walk(PrototypeAST *ast) override;
  virtual void preorder_walk(CallExprAST *ast) override;
  virtual void preorder_walk(ReturnExprAST *ast) override;
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
  virtual void postorder_walk(ReturnExprAST *ast) override;
  virtual void postorder_walk(BinaryExprAST *ast) override;
  virtual void postorder_walk(NumberExprAST *ast) override;
  virtual void postorder_walk(BoolExprAST *ast) override;
  virtual void postorder_walk(VariableExprAST *ast) override;
  virtual void postorder_walk(BlockAST *ast) override;
  virtual void postorder_walk(IfExprAST *ast) override;
  virtual void postorder_walk(TypedVarAST *ast) override;

  void safeguard_visit(AstBase *ast, const std::string &msg) {
    if (ast)
      ast->accept_vis(this);
    else
      this->rep += fmt::format("{} {}", tabs(), msg);
  }
  friend Printable;
};

void ASTPrinter::print(AstBase *ast) {
  auto vs = AstPrinterVisitor();
  ast->accept_vis(&vs);
}
std::string &AstPrinterVisitor::tabs() { return this->current_tabs; }

void AstPrinterVisitor::visit(ProgramAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  for (auto &def : ast->DefinitionVec) {
    safeguard_visit(def.get(), "nullptr DefinitionAST\n");
  }
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(VarDefAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  safeguard_visit(ast->TypedVar.get(), "nullptr TypedVarAST\n");
  safeguard_visit(ast->Expression.get(), "nullptr ExprAST\n");
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(ExternAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  safeguard_visit(ast->Prototype.get(), "nullptr PrototypeAST\n");
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(FuncDefAST *ast) {
  generic_preprintln(ast);
  this->enter_new_scope();
  ast->walk_with_preorder(this);
  safeguard_visit(ast->Prototype.get(), "nullptr PrototypeAST\n");
  safeguard_visit(ast->Block.get(), "nullptr BlockAST\n");
  ast->walk_with_postorder(this);
  this->exit_new_scope();
  generic_postprint();
}

void AstPrinterVisitor::visit(PrototypeAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  for (auto &var : ast->parameterVectors) {
    safeguard_visit(var.get(), "nullptr TypedVarAST\n");
  }
  ast->walk_with_postorder(this);
  generic_postprint();
}
void AstPrinterVisitor::visit(TypedVarAST *ast) {
  this->rep += fmt::format("{} {}: ", tabs(), ast->getTreeName());
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}

void AstPrinterVisitor::visit(CallExprAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  for (auto &arg : ast->arguments) {
    safeguard_visit(arg.get(), "nullptr ExprAST\n");
  }
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(BinaryExprAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  safeguard_visit(ast->LHS.get(), "nullptr ExprAST\n");
  safeguard_visit(ast->RHS.get(), "nullptr ExprAST\n");
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(NumberExprAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(VariableExprAST *ast) {
  generic_preprint(ast);
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(IfExprAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  safeguard_visit(ast->bool_expr.get(), "nullptr ExprAST\n");
  safeguard_visit(ast->thenBlockAST.get(), "nullptr thenBlockAST\n");
  safeguard_visit(ast->elseBlockAST.get(), "nullptr elseBlockAST\n");
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(BoolExprAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::visit(ReturnExprAST *ast) {

  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  safeguard_visit(ast->return_expr.get(), "nullptr ExprAST\n");
  ast->walk_with_postorder(this);
  generic_postprint();
}
void AstPrinterVisitor::visit(BlockAST *ast) {
  generic_preprintln(ast);
  ast->walk_with_preorder(this);
  for (auto &stmt : ast->Statements) {
    safeguard_visit(stmt.get(), "nullptr ExprAST\n");
  }
  ast->walk_with_postorder(this);
  generic_postprint();
}

void AstPrinterVisitor::generic_preprintln(AstBase *ast) {
  if (ast->pe)
    this->rep +=
        fmt::format("{} {} \n", tabs(), ast->getTreeName() + " - ParserError");
  else
    this->rep += fmt::format("{} {} - {}\n", tabs(), ast->getTreeName(),
                             ast->type.to_string());
  current_tabs += "  ";
}
void AstPrinterVisitor::generic_preprint(AstBase *ast) {
  if (ast->pe)
    this->rep +=
        fmt::format("{} {} :", tabs(), ast->getTreeName() + " - ParserError");
  else
    this->rep += fmt::format("{} {} - {} :", tabs(), ast->getTreeName(),
                             ast->type.to_string());
  current_tabs += "  ";
}

void AstPrinterVisitor::generic_postprint() {
  current_tabs.pop_back();
  current_tabs.pop_back();
  this->rep += fmt::format("{}\n", tabs());
}
void AstPrinterVisitor::preorder_walk(ProgramAST *ast) {}
void AstPrinterVisitor::preorder_walk(VarDefAST *ast) {}
void AstPrinterVisitor::preorder_walk(ExternAST *ast) {}
void AstPrinterVisitor::preorder_walk(FuncDefAST *ast) {}
void AstPrinterVisitor::preorder_walk(PrototypeAST *ast) {

  this->rep += fmt::format("{} fn_name: \"{}\"\n", tabs(), ast->functionName);
  this->rep += fmt::format("{} fn_sig: {}\n ", tabs(), ast->type.to_string());
}
void AstPrinterVisitor::preorder_walk(CallExprAST *ast) {}
void AstPrinterVisitor::preorder_walk(ReturnExprAST *ast) {}
void AstPrinterVisitor::preorder_walk(BinaryExprAST *ast) {}
void AstPrinterVisitor::preorder_walk(NumberExprAST *ast) {
  this->rep += fmt::format("(num, type): (\"{}\", {})", ast->number,
                           ast->type.to_string());
}
void AstPrinterVisitor::preorder_walk(BoolExprAST *ast) {}
void AstPrinterVisitor::preorder_walk(VariableExprAST *ast) {
  this->rep += fmt::format("(var_name, type): (\"{}\", {})", ast->variableName,
                           ast->type.to_string());
}
void AstPrinterVisitor::preorder_walk(BlockAST *ast) {}
void AstPrinterVisitor::preorder_walk(IfExprAST *ast) {}
void AstPrinterVisitor::preorder_walk(TypedVarAST *ast) {
  this->rep += fmt::format("(name, type_lxm, type): (\"{}\", \"{}\", {})",
                           ast->name, ast->type_lexeme, ast->type.to_string());
}

// post order
void AstPrinterVisitor::postorder_walk(ProgramAST *ast) {
  while (!this->rep.empty()) {
    auto &ch = this->rep.back();
    if (ch == ' ' || ch == '\n') {
      this->rep.pop_back();
    } else {
      break;
    }
  }
  this->rep += "\n";

  fmt::print("{}", this->rep);
}
void AstPrinterVisitor::postorder_walk(VarDefAST *ast) {}
void AstPrinterVisitor::postorder_walk(ExternAST *ast) {}
void AstPrinterVisitor::postorder_walk(FuncDefAST *ast) {}
void AstPrinterVisitor::postorder_walk(PrototypeAST *ast) {}
void AstPrinterVisitor::postorder_walk(CallExprAST *ast) {}
void AstPrinterVisitor::postorder_walk(ReturnExprAST *ast) {}
void AstPrinterVisitor::postorder_walk(BinaryExprAST *ast) {
  this->rep += fmt::format("{} Operator: \"{}\"", tabs(),
                           ast->Op ? ast->Op->lexeme : " --- ParserError");
}
void AstPrinterVisitor::postorder_walk(NumberExprAST *ast) {}
void AstPrinterVisitor::postorder_walk(BoolExprAST *ast) {}
void AstPrinterVisitor::postorder_walk(VariableExprAST *ast) {}
void AstPrinterVisitor::postorder_walk(BlockAST *ast) {}
void AstPrinterVisitor::postorder_walk(IfExprAST *ast) {}
void AstPrinterVisitor::postorder_walk(TypedVarAST *ast) {}

} // namespace sammine_lang::AST
