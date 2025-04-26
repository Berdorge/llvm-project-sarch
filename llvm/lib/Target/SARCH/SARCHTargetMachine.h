#ifndef LLVM_LIB_TARGET_SARCH_SARCHTARGETMACHINE_H
#define LLVM_LIB_TARGET_SARCH_SARCHTARGETMACHINE_H

#include "SARCHSubtarget.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include <optional>

namespace llvm {
extern Target TheSARCHTarget;

class SARCHTargetMachine : public CodeGenTargetMachineImpl {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  SARCHSubtarget Subtarget;

public:
  SARCHTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                     StringRef FS, const TargetOptions &Options,
                     std::optional<Reloc::Model> RM,
                     std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                     bool JIT);

  const SARCHSubtarget *getSubtargetImpl(const Function &) const override {
    SARCH_DUMP_CYAN
    return &Subtarget;
  }

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override;
};
} // end namespace llvm

#endif // LLVM_LIB_TARGET_SARCH_SARCHTARGETMACHINE_H
