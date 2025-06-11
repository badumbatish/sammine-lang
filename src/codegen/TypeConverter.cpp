#include "codegen/TypeConverter.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
namespace sammine_lang::AST {
llvm::Type *TypeConverter::get_type(Type t) {
  switch (t.type_kind) {
  case TypeKind::I64_t:
    return llvm::Type::getInt64Ty(*context);
  case TypeKind::F64_t:
    return llvm::Type::getDoubleTy(*context);
  case TypeKind::Unit:
    return llvm::Type::getVoidTy(*context);
  case TypeKind::Bool:
    return llvm::Type::getInt1Ty(*context);
  case TypeKind::String:
    return llvm::StructType::get(*context,
                                 llvm::PointerType::getInt8Ty(*context),
                                 llvm::Type::getInt32Ty(*context));
  case TypeKind::Function:
    this->abort("Function is not first-class yet");
  case TypeKind::NonExistent:
    this->abort("Existed a type that is not synthesized yet");
  case TypeKind::Poisoned:
    this->abort("Poisoned typed should not be here");
  }
  this->abort("Guarded by default case");
}
llvm::Type *TypeConverter::get_return_type(Type t) {

  switch (t.type_kind) {
  case TypeKind::Function:
    return get_type(std::get<FunctionType>(t.type_data).get_return_type());
  default:
    this->abort("Jasmine passed in something that is not a function type");
    break;
  }
  this->abort("Guarded by default case");
}
llvm::CmpInst::Predicate TypeConverter::get_cmp_func(Type a, Type b,
                                                     TokenType tok) {
  this->abort_if_not(a.type_kind == b.type_kind,
                     "Two types needs to be the same");
  using llvm::CmpInst;

  switch (a.type_kind) {

  case TypeKind::I64_t:
  case TypeKind::Bool: {
    // Signed integer comparisons
    switch (tok) {
    case TokenType::TokEQUAL:
      return CmpInst::ICMP_EQ;
    // case TokenType::TokNOTEqual:
    //   return CmpInst::ICMP_ONE;
    case TokenType::TokLESS:
      return CmpInst::ICMP_SLT;
    case TokenType::TokLessEqual:
      return CmpInst::ICMP_SLE;
    case TokenType::TokGREATER:
      return CmpInst::ICMP_SGT;
    case TokenType::TokGreaterEqual:
      return CmpInst::ICMP_SGE;
    default:

      this->abort("Invalid token for integer comparison");
    }
    break;
  }
  case TypeKind::F64_t: {
    // Ordered floating-point comparisons
    switch (tok) {
    case TokenType::TokEQUAL:
      return CmpInst::FCMP_OEQ;
    // case TokenType::TokNOTEqual:
    //   return CmpInst::FCMP_ONE;
    case TokenType::TokLESS:
      return CmpInst::FCMP_OLT;
    case TokenType::TokLessEqual:
      return CmpInst::FCMP_OLE;
    case TokenType::TokGREATER:
      return CmpInst::FCMP_OGT;
    case TokenType::TokGreaterEqual:
      return CmpInst::FCMP_OGE;
    default:
      this->abort("Invalid token for float comparison");
    }
    break;
  }
  case TypeKind::Unit:
    this->abort(
        fmt::format("Cannot compare values of this type: {}", a.to_string()));
  case TypeKind::Function:
    this->abort(
        fmt::format("Cannot compare values of this type: {}", a.to_string()));
  case TypeKind::NonExistent:
    this->abort(
        fmt::format("Cannot compare values of this type: {}", a.to_string()));
  case TypeKind::Poisoned:
    this->abort(
        fmt::format("Cannot compare values of this type: {}", a.to_string()));
    break;
  case TypeKind::String:
    this->abort(
        fmt::format("Cannot compare values of this type: {}", a.to_string()));
    break;
  }
  this->abort("End of get_cmp_func reached");
}
} // namespace sammine_lang::AST
