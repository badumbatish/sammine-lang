#include "ast/Ast.h"
#include "fmt/format.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include <codegen/Garbage.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>

namespace sammine_lang::AST {
/// Insert a FrameMap in the beginning of each function
void ShadowGarbageCollector::createFrameMapForCallee(FuncDefAST *f) {
  /// The map for a single function's stack frame.  One of these is
  ///        compiled as constant data into the executable for each function.
  ///
  /// Storage of metadata values is elided if the %metadata parameter to
  /// @llvm.gcroot is null.
  // struct FrameMap {
  //   int32_t NumRoots;    //< Number of roots in stack frame.
  //   int32_t NumMeta;     //< Number of metadata entries.  May be <
  //   NumRoots. const void ShadowGarbageCollector:: *Meta[0]; //< Metadata for
  //   each root.
  // };
  auto fm = llvm::StructType::create(
      context, fmt::format("{}_{}", std::string(f->Prototype->functionName),
                           "FrameMap"));
  llvm::PointerType *int8ptr =
      llvm::PointerType::get(llvm::Type::getInt8Ty(context),
                             0); // 0 stands for generic address space
  llvm::ArrayType *MetaArrayTy =
      llvm::ArrayType::get(int8ptr, MetaDataEntries.size());
  // llvm::Constant *MetaArray =
  //     llvm::ConstantArray::get(MetaArrayTy, MetaDataEntries);
  fm->setBody({llvm::Type::getInt32Ty(context), llvm::Type::getInt32Ty(context),
               MetaArrayTy});

  // TODO: Insert this into global data.
  // llvm::GlobalVariable(module, fm, true, llvm::GlobalValue::ExternalLinkage,
  //                      /* each function's num root and num meta here here
  //                      */ );
}
void ShadowGarbageCollector::setStackEntryFromCaller(FuncDefAST *f) {

  /// INFO: We'll set the global curr_stack_entry->next to be the caller,
  /// We'll also set the curr_stack_entry->frame_map to be the callee's frame
  /// map
  ///
  /// INFO: this will require us to keep a map of string to FrameMap in the
  /// ShadowGarbageCollector class.
  /// For more details, see createFrameMap
  ///
  ///

  /// A link in the dynamic shadow stack.  One of these is embedded in
  ///        the stack frame of each function on the call stack.
  // struct StackEntry {
  //   StackEntry *Next;    //< Link to next stack entry (the caller's).
  //   const FrameMap *Map; //< Pointer to constant FrameMap.
  //   void ShadowGarbageCollector:: *Roots[0];      //< Stack roots (in-place
  //   array).
  // };

  auto se = llvm::StructType::create(context, "stack_entrry");
  llvm::PointerType *int8ptr =
      llvm::PointerType::get(llvm::Type::getInt8Ty(context),
                             0); // 0 stands for generic address space

  llvm::ArrayType *MetaArrayTy =
      llvm::ArrayType::get(int8ptr, MetaDataEntries.size());
  // llvm::Constant *MetaArray =
  //     llvm::ConstantArray::get(MetaArrayTy, MetaDataEntries);
  se->setBody({llvm::Type::getInt32Ty(context), llvm::Type::getInt32Ty(context),
               MetaArrayTy});
  // TODO: Insert this into the linked list
  // builder.Insert(se);
}

void ShadowGarbageCollector::initGlobalRootChain() {
  /// The head of the singly-linked list of StackEntries.  Functions push
  ///        and pop onto this in their prologue and epilogue.
  ///
  /// Since there is only a global list, this technique is not threadsafe.
  // StackEntry *llvm_gc_root_chain;
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
int32_t NumRootCalculator::calculate(VarDefAST *ast) {
  return calculate(ast->Expression.get());
}

} // namespace sammine_lang::AST
