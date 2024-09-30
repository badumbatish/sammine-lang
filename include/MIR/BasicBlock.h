#pragma once
#include "AST/Ast.h"
#include "Type.h"
#include "Utilities.h"
#include <memory>
#include <vector>
namespace sammine_lang::MIR {
using sammine_util::Location;
class Instruction {
  std::shared_ptr<Location> location;
};

class InsAlloca : public Instruction {
public:
  Type type;
};
class InsAdd : public Instruction {
public:
  Type type;
  std::shared_ptr<AST::ExprAST> left_operand, right_operand, destination;
};

class InsSub : public Instruction {
public:
  Type type;
  std::shared_ptr<AST::ExprAST> left_operand, right_operand, destination;
};

class InsMul : public Instruction {
public:
  Type type;
  std::shared_ptr<AST::ExprAST> left_operand, right_operand, destination;
};

class InsDiv : public Instruction {
public:
  Type type;
  std::shared_ptr<AST::ExprAST> left_operand, right_operand, destination;
};

class Load : public Instruction {
public:
  Type type;
  std::shared_ptr<AST::ExprAST> left_operand, right_operand, destination;
};

class InsConstant : public Instruction {
public:
  Type type;
};

class InsCall : public Instruction {
public:
  Type type;
  std::vector<std::shared_ptr<AST::ExprAST>> args;
  std::string function_name;
};

class BasicBlock {
public:
  bool is_filled;
  bool is_sealed;
  std::vector<Instruction> instructions;
  std::vector<std::shared_ptr<BasicBlock>> predecessors;
  std::vector<std::shared_ptr<BasicBlock>> successors;
  static void link_basic_block(std::shared_ptr<BasicBlock> from,
                               std::shared_ptr<BasicBlock> to);
};

class InsPhi : public Instruction {
  std::vector<std::pair<std::string, std::shared_ptr<BasicBlock>>> merge_points;
  std::string name;
};

} // namespace sammine_lang::MIR
