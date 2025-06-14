#include "ast/Ast.h"
#include "codegen/CodegenUtils.h"
#include "fmt/format.h"
#include "util/Utilities.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include <codegen/Garbage.h>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Value.h>
#include <memory>

namespace sammine_lang::AST {
/// Insert a FrameMap in the beginning of each function
llvm::GlobalVariable *
ShadowGarbageCollector::getFrameMapForCallee(FuncDefAST *f) {
  std::string frame_map_name =
      fmt::format("{}_{}", std::string(f->Prototype->functionName), "FrameMap");
  if (auto fm = module.getGlobalVariable(frame_map_name))
    return fm;
  /// The map for a single function's stack frame.  One of these is
  ///        compiled as constant data into the executable for each function.
  ///
  /// Storage of metadata values is elided if the %metadata parameter to
  /// @llvm.gcroot is null.
  // struct FrameMap {
  //   int32_t NumRoots;    //< Number of roots in stack frame.
  //   int32_t NumMeta;     //< Number of metadata entries.  May be <
  //   NumRoots. const void ShadowGarbageCollector:: *Meta[0]; //< Metadata
  //   for each root.
  // };
  auto fm = llvm::StructType::create(context, "frame_map_type");
  llvm::PointerType *int64ptr =
      llvm::PointerType::get(llvm::Type::getInt64Ty(context),
                             0); // 0 stands for generic address space
  fm->setBody(int64ptr);

  llvm::Constant *frame_map_init = llvm::ConstantStruct::get(
      fm, llvm::ConstantInt::get(
              context,
              llvm::APInt(64, NumRootCalculator::calculate(f->Block.get()))));
  return new llvm::GlobalVariable(
      module, fm, true, llvm::GlobalValue::ExternalLinkage, frame_map_init, "");
}
void ShadowGarbageCollector::relieveStackEntryByCallee(FuncDefAST *f,
                                                       llvm::Function *llvm_f) {
  // allocate a struct to store the current global root chain info

  auto *stackEntryTy = GetStackEntryType(); // StructType*

  auto *global_root_chain = module.getGlobalVariable(GLOBAL_ROOT_CHAIN);
  auto stacked_global_root_chain =
      builder.CreateLoad(stackEntryTy, global_root_chain);

  if (stacked_global_root_chain) {
  }
}

void ShadowGarbageCollector::setStackEntryFromCaller(FuncDefAST *f,
                                                     llvm::Function *llvm_f) {

  /// INFO: We'll set the global curr_stack_entry->next to be the caller,
  /// We'll also set the curr_stack_entry->frame_map to be the callee's frame
  /// map
  ///
  /// INFO: this will require us to keep a map of string to FrameMap in the
  /// ShadowGarbageCollector class.
  /// For more details, see createFrameMap
  ///
  ///
  auto *stackEntryTy = GetStackEntryType(); // StructType*
  auto *callee_stack_entry =
      CodegenUtils::CreateEntryBlockAlloca(llvm_f, "stack_entry", stackEntryTy);

  // 1. Get global_root_chain
  llvm::GlobalVariable *globalRootChain =
      module.getNamedGlobal(GLOBAL_ROOT_CHAIN);
  assert(globalRootChain && "global_root_chain not defined");

  // 2. store the address of global root chain to the first element of of
  // callee_stack_entry (shadow stack)
  // INFO: Only store this if this is not the entry point, a.k.a the main
  // function
  if (!CodegenUtils::isFunctionMain(f)) {
    llvm::Value *nextFieldPtr = builder.CreateStructGEP(
        stackEntryTy, callee_stack_entry, 0, "next_field_ptr");
    builder.CreateStore(globalRootChain, nextFieldPtr, "old_head");
  }

  // 3. store the address of global frame map to the second element of
  // callee_stack_entry

  llvm::Value *frameMapPtr = builder.CreateStructGEP(
      stackEntryTy, callee_stack_entry, 1, "framemap_field_ptr");
  auto frame_map = this->getFrameMapForCallee(f);
  builder.CreateStore(frame_map, frameMapPtr);

  // Make the current stack entry to be the global_root_chain
  // globalRootChain->setInitialize
  builder.CreateStore(callee_stack_entry, globalRootChain);
}

llvm::StructType *ShadowGarbageCollector::GetStackEntryType() {
  // struct StackEntry {
  //   StackEntry *Next;    //< Link to next stack entry (the caller's).
  //   const FrameMap *Map; //< Pointer to constant FrameMap.
  //   void ShadowGarbageCollector:: *Roots[0];      //< Stack roots (in-place
  //   array).
  // };
  //
  if (auto set = llvm::StructType::getTypeByName(context, "stack_entry_type"))
    return set;

  auto se = llvm::StructType::create(context, "stack_entry_type");
  llvm::PointerType *int64ptr =
      llvm::PointerType::get(llvm::Type::getInt64Ty(context),
                             0); // 0 stands for generic address space

  // llvm::ArrayType *MetaArrayTy =
  //     llvm::ArrayType::get(int8ptr, MetaDataEntries.size());
  // llvm::Constant *MetaArray =
  //     llvm::ConstantArray::get(MetaArrayTy, MetaDataEntries);
  se->setBody({int64ptr, int64ptr});

  return se;
}
void ShadowGarbageCollector::initGlobalRootChain() {
  /// The head of the singly-linked list of StackEntries.  Functions push
  ///        and pop onto this in their prologue and epilogue.
  ///
  /// Since there is only a global list, this technique is not threadsafe.
  // StackEntry *llvm_gc_root_chain;
  // INFO: Create a stack entry
  auto global_root_chain_type = GetStackEntryType();

  // INFO: Create a struct constant so we can use it to initialize the global
  // root chain
  auto *null_stack_entry =
      global_root_chain_type->getElementType(0); // StackEntry*
  auto *null_frame_map = global_root_chain_type->getElementType(1); //

  auto null_entry_ptr = llvm::ConstantPointerNull::get(
      llvm::cast<llvm::PointerType>(null_stack_entry));
  auto null_frame_map_ptr = llvm::ConstantPointerNull::get(
      llvm::cast<llvm::PointerType>(null_frame_map));
  llvm::Constant *stack_entry_initializer = llvm::ConstantStruct::get(
      global_root_chain_type, {null_entry_ptr, null_frame_map_ptr}
      // ignore Roots[0] since it's unsized
  );

  // INFO: Put it in the module as a global root
  //
  // INFO: Don't worry about a raw new here, the module takes ownership of it,
  //
  // INFO: We use new here and not unique ptr because we don't own it.
  //      We don't use local stack variable because if we exit this scope, we
  //      lose the global variables handled by the module
  //      We don't use shared ptr because we need the module to be the one
  //      decomissioning it
  new llvm::GlobalVariable(module, global_root_chain_type,
                           /* isConstant*/ false,
                           llvm::GlobalValue::ExternalLinkage,
                           stack_entry_initializer, GLOBAL_ROOT_CHAIN);
}

void ShadowGarbageCollector::initGCFunc() {
  /// Calls Visitor(root, meta) for each GC root on the stack.
  ///        root and meta are exactly the values passed to
  ///        @llvm.gcroot.
  ///
  /// Visitor could be a function to recursively mark live objects.  Or it
  /// might copy them to another heap or generation.
  ///
  /// @param Visitor A function to invoke for every GC root on the stack.
  // void ShadowGarbageCollector:: visitGCRoots(void (*Visitor)(void **Root,
  // const void *Meta)) {
  //   for (StackEntry *R = llvm_gc_root_chain; R; R = R->Next) {
  //     unsigned i = 0;
  //
  //     // For roots [0, NumMeta), the metadata pointer is in the FrameMap.
  //     for (unsigned e = R->Map->NumMeta; i != e; ++i)
  //       Visitor(&R->Roots[i], R->Map->Meta[i]);
  //
  //     // For roots [NumMeta, NumRoots), the metadata pointer is null.
  //     for (unsigned e = R->Map->NumRoots; i != e; ++i)
  //       Visitor(&R->Roots[i], NULL);
  //   }
}

int32_t NumRootCalculator::calculate(BlockAST *ast) {
  int num_roots = 0;
  for (auto &e : ast->Statements) {
    num_roots += calculate(e.get());
  }

  return num_roots;
}
int32_t NumRootCalculator::calculate(ExprAST *ast) {
  if (auto e = dynamic_cast<IfExprAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<VariableExprAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<CallExprAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<ReturnExprAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<BoolExprAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<StringExprAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<NumberExprAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<VarDefAST *>(ast)) {
    return calculate(e);
  } else if (auto e = dynamic_cast<UnitExprAST *>(ast)) {
    return calculate(e);
  } else {
    sammine_util::abort(
        fmt::format("You should be overloading on {}", ast->getTreeName()));
    return 0;
  }
}
int32_t NumRootCalculator::calculate(IfExprAST *ast) {
  return calculate(ast->bool_expr.get()) + calculate(ast->thenBlockAST.get()) +
         calculate(ast->elseBlockAST.get());
}
int32_t NumRootCalculator::calculate(VariableExprAST *ast) { return 0; }

// TODO: Tell Jasmine to re-check this
int32_t NumRootCalculator::calculate(CallExprAST *ast) {
  int32_t num_roots = 0;
  for (auto &arg : ast->arguments) {
    num_roots += calculate(arg.get());
  }
  return num_roots;
}

int32_t NumRootCalculator::calculate(ReturnExprAST *ast) {
  return calculate(ast->return_expr.get());
}
int32_t NumRootCalculator::calculate(BinaryExprAST *ast) {
  auto left = calculate(ast->LHS.get());
  auto right = calculate(ast->RHS.get());
  return left + right;
}
int32_t NumRootCalculator::calculate(BoolExprAST *ast) { return 0; }
int32_t NumRootCalculator::calculate(StringExprAST *ast) { return 1; }
int32_t NumRootCalculator::calculate(NumberExprAST *ast) { return 0; }
int32_t NumRootCalculator::calculate(UnitExprAST *ast) { return 0; }
int32_t NumRootCalculator::calculate(VarDefAST *ast) {
  return calculate(ast->Expression.get());
}

} // namespace sammine_lang::AST
