#include "fmt/format.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include <codegen/Garbage.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>

/// Insert a FrameMap in the beginning of each function
void ShadowGarbageCollector::createFrameMap(llvm::Function *f) {
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
      context, fmt::format("{}_{}", std::string(f->getName()), "FrameMap"));
  llvm::PointerType *int8ptr =
      llvm::PointerType::get(llvm::Type::getInt8Ty(context),
                             0); // 0 stands for generic address space
  llvm::ArrayType *MetaArrayTy =
      llvm::ArrayType::get(int8ptr, MetaDataEntries.size());
  // llvm::Constant *MetaArray =
  //     llvm::ConstantArray::get(MetaArrayTy, MetaDataEntries);
  fm->setBody(llvm::Type::getInt32Ty(context), llvm::Type::getInt32Ty(context),
              MetaArrayTy);

  // TODO: Insert this into global data.
  // llvm::GlobalVariable(module, fm, true, llvm::GlobalValue::ExternalLinkage,
  //                      /* each function's num root and num meta here here
  //                      */);
}
void ShadowGarbageCollector::createStackEntry(llvm::Function *f) {
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
  se->setBody(llvm::Type::getInt32Ty(context), llvm::Type::getInt32Ty(context),
              MetaArrayTy);
  // TODO: Insert this in the front of a function
  builder.Insert(se);
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
