#pragma once
#include "AST/Ast.h"
#include "BasicBlock.h"
#include "Utilities.h"
#include <set>
namespace sammine_lang::MIR {

class CFG {
  std::set<BasicBlock> cfg;
  sammine_util::ErrorMsgs error_msgs;
  CFG(std::shared_ptr<AST::ProgramAST> ast);
};

} // namespace sammine_lang::MIR
