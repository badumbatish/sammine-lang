
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

#include "ast/AstDecl.h"
namespace sammine_lang {
using namespace AST;
class CodegenUtils {

public:
  static llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction,
                                                  const std::string &VarName,
                                                  llvm::Type *);

  static bool isFunctionMain(FuncDefAST *);
};
} // namespace sammine_lang
