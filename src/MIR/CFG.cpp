#include "MIR/CFG.h"

namespace sammine_lang::MIR {
// For each assignment in the AST, we need to track them down via braun
//
//
// Let's see.
// we set down a phi instruction.
// we recursive go back and check for definitions
// in our predecessors
CFG::CFG(std::shared_ptr<AST::ProgramAST> ast) {
  for (auto &function : ast->DefinitionVec) {
  }
}
} // namespace sammine_lang::MIR
