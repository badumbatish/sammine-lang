#include "codegen/CodegenUtils.h"
#include <llvm/IR/IRBuilder.h>

#include "ast/Ast.h"
namespace sammine_lang {

llvm::AllocaInst *CodegenUtils::CreateEntryBlockAlloca(
    llvm::Function *Function, const std::string &VarName, llvm::Type *type) {

  llvm::IRBuilder<> TmpB(&Function->getEntryBlock(),
                         Function->getEntryBlock().begin());
  return TmpB.CreateAlloca(type, nullptr, VarName);
}
bool CodegenUtils::isFunctionMain(FuncDefAST *ast) {
  return ast->Prototype->functionName == "main";
}

} // namespace sammine_lang
