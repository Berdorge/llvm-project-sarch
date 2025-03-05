#include "TargetInfo/SARCHTargetInfo.h"
#include "SARCH.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheSARCHTarget() {
  SARCH_DUMP_YELLOW
  static Target TheSARCHTarget;
  return TheSARCHTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSARCHTargetInfo() {
  SARCH_DUMP_YELLOW
  RegisterTarget<Triple::sarch> X(getTheSARCHTarget(), "SARCH",
                                  "Student architecture target for LLVM course",
                                  "SARCH");
}
