#include "SARCHTargetMachine.h"
#include "SARCH.h"
#include "TargetInfo/SARCHTargetInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include <optional>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSARCHTarget() {
  SARCH_DUMP_CYAN
  RegisterTargetMachine<SARCHTargetMachine> A(getTheSARCHTarget());
}

SARCHTargetMachine::SARCHTargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       std::optional<Reloc::Model> RM,
                                       std::optional<CodeModel::Model> CM,
                                       CodeGenOptLevel OL, bool JIT)
    : CodeGenTargetMachineImpl(T, "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32",
                               TT, CPU, FS, Options, Reloc::Static,
                               getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  SARCH_DUMP_CYAN
  initAsmInfo();
}

namespace {
class SARCHPassConfig : public TargetPassConfig {
public:
  SARCHPassConfig(SARCHTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  SARCHTargetMachine &getSARCHTargetMachine() const {
    return getTM<SARCHTargetMachine>();
  }

  bool addInstSelector() override {
    SARCH_DUMP_CYAN
    addPass(createSARCHISelDag(getSARCHTargetMachine(), getOptLevel()));
    return false;
  }
};
} // namespace

TargetPassConfig *SARCHTargetMachine::createPassConfig(PassManagerBase &PM) {
  SARCH_DUMP_CYAN
  return new SARCHPassConfig(*this, PM);
}

TargetLoweringObjectFile *SARCHTargetMachine::getObjFileLowering() const {
  SARCH_DUMP_CYAN
  return TLOF.get();
}
