//
// Created by Jasmine Tang on 3/27/24.
//

#include "CodegenVisitor.h"

namespace sammine_lang::AST {
void CgVisitor::visit(ProgramAST *ast) {
  for(auto& def : ast->DefinitionVec) {
    def->accept_vis(this);
  }
}
void CgVisitor::visit(VarDefAST *ast) {}
}
