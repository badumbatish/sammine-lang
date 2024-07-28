//
// Created by Jasmine Tang on 3/27/24.
//

#ifndef SAMMINE_LANG_CODEGENVISITOR_H
#define SAMMINE_LANG_CODEGENVISITOR_H

#include "Ast.h"
#include "AstBase.h"
#include "LLVMRes.h"
#include <memory>
namespace sammine_lang::AST {
class CgVisitor : public ASTVisitor {
private:
  std::shared_ptr<sammine_lang::LLVMRes> resPtr;

public:
  CgVisitor(std::shared_ptr<sammine_lang::LLVMRes> resPtr) : resPtr(resPtr) {}
  void visit(Visitable *visitable) {}
};
} // namespace sammine_lang::AST

#endif // SAMMINE_LANG_CODEGENVISITOR_H
