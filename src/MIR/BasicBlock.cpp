#include "MIR/BasicBlock.h"

namespace sammine_lang::MIR {
static void link_basic_block(std::shared_ptr<BasicBlock> from,
                             std::shared_ptr<BasicBlock> to) {
  if (from == nullptr || to == nullptr)
    sammine_util::abort("ERROR in link_basic_block(...). One of the pointer "
                        "is null. This should never happen");

  from->successors.push_back(to);
  to->successors.push_back(from);
}

} // namespace sammine_lang::MIR
